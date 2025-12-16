#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORT 8080
#define BUF_SIZE 1024
#define OUTPUT_FILE "received.txt"

int main() {
    int sock, file_fd;
    struct sockaddr_in server;
    char buffer[BUF_SIZE];
    long filesize, received = 0;
    ssize_t n;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(sock, (struct sockaddr *)&server, sizeof(server));

    
    read(sock, &filesize, sizeof(filesize));

    
    file_fd = open(OUTPUT_FILE, O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (file_fd < 0) {
        perror("File create failed");
        close(sock);
        return 1;
    }

    while (received < filesize) {
        n = read(sock, buffer, BUF_SIZE);
        write(file_fd, buffer, n);
        received += n;
    }

    printf("File received successfully\n");

    close(file_fd);
    close(sock);
    return 0;
}

