#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024

int main() {
    int sock;
    struct sockaddr_in server;
    char buffer[BUF_SIZE];
    char operation[BUF_SIZE];
    int a, b, result;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server, sizeof(server));
    printf("Connected to server\n");

    while (1) {
         printf("in while loop \n");
        /* Receive operation */
        memset(operation, 0, BUF_SIZE);
        if (recv(sock, operation, BUF_SIZE, 0) <= 0)
            break;

        /* Receive operands */
        memset(buffer, 0, BUF_SIZE);
        recv(sock, buffer, BUF_SIZE, 0);
        sscanf(buffer, "%d %d", &a, &b);

        if (strcmp(operation, "ADD") == 0)
            result = a + b;
        else if (strcmp(operation, "SUB") == 0)
            result = a - b;
        else
            break;

        snprintf(buffer, BUF_SIZE, "%d", result);
        send(sock, buffer, strlen(buffer), 0);
    }

    close(sock);
    return 0;
}

