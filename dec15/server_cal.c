#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    char buffer[BUF_SIZE];
    int a, b, choice;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 1);

    printf("Server listening on port %d...\n", PORT);
    client_fd = accept(server_fd, NULL, NULL);
    printf("Client connected\n");

    while (1) {
        printf("\n1. Add\n2. Subtract\nEnter choice: ");
        scanf("%d", &choice);

        printf("Enter two numbers: ");
        scanf("%d %d", &a, &b);

        /* Send operation */
        if (choice == 1)
            strcpy(buffer, "ADD");
        else if (choice == 2)
            strcpy(buffer, "SUB");
        else
            break;

        send(client_fd, buffer, strlen(buffer), 0);
          
        printf("check 2\n");
        /* Send operands */
        snprintf(buffer, BUF_SIZE, "%d %d", a, b);
        send(client_fd, buffer, strlen(buffer), 0);
          
        printf("check 3\n");
        /* Receive result */
        memset(buffer, 0, BUF_SIZE);
        recv(client_fd, buffer, BUF_SIZE, 0);
        printf("Result from client: %s\n", buffer);
    }

    close(client_fd);
    close(server_fd);
    return 0;
}

