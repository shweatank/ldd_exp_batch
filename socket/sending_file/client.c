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

    // Open file to send
    FILE *fp = fopen("file.txt", "r");
    if (!fp) {
        perror("file open");
        exit(1);
    }

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        exit(1);
    }

    // Read file and send data
    while (fgets(buffer, BUF_SIZE, fp) != NULL) {
        if (send(sock, buffer, strlen(buffer), 0) == -1) {
            perror("send");
            exit(1);
        }
    }

    printf("File sent successfully.\n");

    fclose(fp);
    close(sock);
    return 0;
}

