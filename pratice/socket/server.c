#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9090
#define BUF_SIZE 1024

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUF_SIZE];

    /* Create socket */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket");
        exit(1);
    }

    /* Bind */
    server_addr.sin_family = AF_INET;
    //server_addr.sin_addr.s_addr = INADDR_ANY;   // 127.0.0.1
    server_addr.sin_addr.s_addr = inet_addr("10.53.202.102");
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    /* Listen */
    if (listen(server_fd, 1) < 0) {
        perror("listen");
        exit(1);
    }

    printf("Server listening on port %d...\n", PORT);

    /* Accept */
    client_fd = accept(server_fd,
                       (struct sockaddr *)&client_addr,
                       &addr_len);
    if (client_fd < 0) {
        perror("accept");
        exit(1);
    }

    printf("Client connected.\n");

    /* Infinite send/receive loop */
    while (1) {
        memset(buffer, 0, BUF_SIZE);

        /* Receive from client */
        int n = recv(client_fd, buffer, BUF_SIZE, 0);
        if (n <= 0) {
            printf("Client disconnected\n");
            break;
        }

        printf("Client: %s\n", buffer);

        /* Send response */
        snprintf(buffer, BUF_SIZE, "ACK from server");
        send(client_fd, buffer, strlen(buffer), 0);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
