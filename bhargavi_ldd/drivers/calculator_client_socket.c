#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define BUF_SIZE 1024

int main()
{
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];

    /* Create socket */
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    /* Server address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);

    /* Convert IP address */
    if (inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(1);
    }

    /* Connect to server */
    printf("Connecting to calculator server...\n");
    
    if (connect(sock, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0) {
        perror("connect");
        printf("Make sure server is running first!\n");
        exit(1);
    }

    printf("Connected to server successfully!\n\n");

    /* Receive welcome message */
    memset(buffer, 0, BUF_SIZE);
    recv(sock, buffer, BUF_SIZE, 0);
    printf("%s", buffer);

    /* Main interaction loop */
    while (1) {
        printf("Enter expression: ");
        
        /* Get user input */
        if (fgets(buffer, BUF_SIZE, stdin) == NULL) {
            break;
        }

        /* Remove newline */
        buffer[strcspn(buffer, "\n")] = 0;

        /* Check for exit */
        if (strcmp(buffer, "exit") == 0 || strcmp(buffer, "quit") == 0) {
            /* Send exit command to server */
            send(sock, buffer, strlen(buffer), 0);
            printf("Disconnecting from server...\n");
            break;
        }

        /* Skip empty input */
        if (strlen(buffer) == 0) {
            continue;
        }

        /* Send expression to server */
        send(sock, buffer, strlen(buffer), 0);

        /* Receive response from server */
        memset(buffer, 0, BUF_SIZE);
        int n = recv(sock, buffer, BUF_SIZE, 0);
        if (n <= 0) {
            printf("Server disconnected\n");
            break;
        }

        /* Display result */
        printf("Server: %s", buffer);
    }

    /* Cleanup */
    close(sock);
    printf("Client shutdown\n");

    return 0;
}
