#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define BUFFER_SIZE 1024
#define MAX_FILENAME 256

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    
    printf("=== File Transfer Client ===\n");
    printf("Connecting to server...\n");
    
    /* Create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }
    
    /* Server address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    
    /* Connect to server */
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(1);
    }
    
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        printf("Make sure server is running first!\n");
        exit(1);
    }
    
    printf("Connected to server successfully!\n\n");
    
    /* Receive welcome message */
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);
    if (bytes_received > 0) {
        printf("%s", buffer);
    }
    
    /* Receive file info */
    memset(buffer, 0, BUFFER_SIZE);
    bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);
    if (bytes_received <= 0) {
        printf("No file info received\n");
        close(sock);
        return 1;
    }
    
    /* Parse filename and size */
    char *token = strtok(buffer, ":");
    if (token == NULL || strcmp(token, "FILE") != 0) {
        printf("Invalid file info received: %s\n", buffer);
        close(sock);
        return 1;
    }
    
    char filename[MAX_FILENAME];
    long file_size;
    
    token = strtok(NULL, ":");
    if (token) {
        strncpy(filename, token, MAX_FILENAME - 1);
        filename[MAX_FILENAME - 1] = '\0';
    } else {
        strcpy(filename, "received_file.bin");
    }
    
    token = strtok(NULL, ":");
    if (token) {
        file_size = atol(token);
    } else {
        file_size = 0;
    }
    
    printf("Receiving file: %s\n", filename);
    printf("File size: %ld bytes\n", file_size);
    
    /* Send ACK for file info */
    char ack_msg[BUFFER_SIZE];
    snprintf(ack_msg, sizeof(ack_msg), "ACK: Ready to receive file '%s'\n", filename);
    send(sock, ack_msg, strlen(ack_msg), 0);
    
    /* Open file for writing */
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error creating file '%s'\n", filename);
        close(sock);
        return 1;
    }
    
    /* Receive file in chunks */
    long total_received = 0;
    int chunk_count = 0;
    int eof_received = 0;
    
    printf("\nStarting file reception...\n");
    
    while (!eof_received) {
        memset(buffer, 0, BUFFER_SIZE);
        
        /* Receive chunk from server */
        bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);
        
        if (bytes_received <= 0) {
            printf("Server disconnected\n");
            break;
        }
        
        /* Check for EOF marker (must be exact match and separate packet) */
        if (bytes_received == 3 && strncmp(buffer, "EOF", 3) == 0) {
            printf("Received EOF marker\n");
            eof_received = 1;
            continue;  // Skip writing EOF to file
        }
        
        /* Write chunk to file */
        size_t bytes_written = fwrite(buffer, 1, bytes_received, file);
        if (bytes_written != bytes_received) {
            printf("Error writing to file\n");
            break;
        }
        
        total_received += bytes_received;
        chunk_count++;
        
        printf("Received chunk %d: %d bytes (Total: %ld", 
               chunk_count, bytes_received, total_received);
        
        if (file_size > 0) {
            printf("/%ld", file_size);
        }
        printf(")\n");
        
        /* Send ACK for chunk */
        snprintf(ack_msg, sizeof(ack_msg), 
                 "ACK: Received chunk %d (%d bytes). Total: %ld", 
                 chunk_count, bytes_received, total_received);
        
        if (file_size > 0) {
            strncat(ack_msg, "/", BUFFER_SIZE - strlen(ack_msg) - 1);
            char size_str[20];
            snprintf(size_str, sizeof(size_str), "%ld", file_size);
            strncat(ack_msg, size_str, BUFFER_SIZE - strlen(ack_msg) - 1);
        }
        
        strncat(ack_msg, "\n", BUFFER_SIZE - strlen(ack_msg) - 1);
        send(sock, ack_msg, strlen(ack_msg), 0);
    }
    
    /* File transfer complete */
    fclose(file);
    
    printf("\nFile reception complete!\n");
    printf("Total chunks: %d\n", chunk_count);
    printf("Total bytes received: %ld\n", total_received);
    
    /* Verify file size */
    char final_ack[BUFFER_SIZE];
    if (file_size > 0) {
        if (total_received == file_size) {
            printf("File verification: SUCCESS (Size matches)\n");
            snprintf(final_ack, sizeof(final_ack), 
                     "ACK: File '%s' received successfully. Size: %ld bytes\n", 
                     filename, total_received);
        } else {
            printf("File verification: FAILED (Expected: %ld, Received: %ld)\n", 
                   file_size, total_received);
            snprintf(final_ack, sizeof(final_ack), 
                     "ACK: File received with size mismatch. Expected: %ld, Got: %ld\n", 
                     file_size, total_received);
        }
    } else {
        printf("File received successfully\n");
        snprintf(final_ack, sizeof(final_ack), 
                 "ACK: File '%s' received. Size: %ld bytes\n", 
                 filename, total_received);
    }
    
    /* Send final ACK */
    send(sock, final_ack, strlen(final_ack), 0);
    
    /* Receive completion message */
    memset(buffer, 0, BUFFER_SIZE);
    bytes_received = recv(sock, buffer, BUFFER_SIZE, 0);
    if (bytes_received > 0) {
        printf("Server: %s", buffer);
    }
    
    /* Cleanup */
    close(sock);
    printf("\nClient shutdown\n");
    
    return 0;
}
