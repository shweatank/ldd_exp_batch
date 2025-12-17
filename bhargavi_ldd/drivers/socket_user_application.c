#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
 
#define PORT 9000
#define BUF_SIZE 1024
 
struct data{
    int n1;
    int n2;
    char op;
};
 
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
    inet_pton(AF_INET, "192.168.0.135", &server_addr.sin_addr);
 
    /* Connect */
    if (connect(sock, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0) {
        perror("connect");
        exit(1);
    }
 
    printf("Connected to server.\n");
 
    /* Infinite send/receive loop */
    while (1) {
        /* Send message */
        struct data d;
        printf("Enter op: ");
        scanf("%c", &d.op);
        printf("Enter numbers: ");
        scanf("%d %d", &d.n1, &d.n2);
        send(sock, (void *)&d, sizeof(d), 0);
 
        /* Receive response */
        int res;
        int n = recv(sock, (void *)&res, 4, 0);
        if (n <= 0) {
            printf("Server disconnected\n");
            break;
        }
 
        printf("Result: %d\n", res);
        getchar();
    }
 
    close(sock);
    return 0;
}
