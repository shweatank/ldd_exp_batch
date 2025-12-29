#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT     5000
#define BUF_SIZE 4096

int main(void)
{
    int server_fd, client_fd;
    struct sockaddr_in addr;
    FILE *fp;
    char buffer[BUF_SIZE];
    ssize_t n;
    uint32_t filesize_net, filesize;
    size_t total = 0;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 1);

    printf("Waiting for connection...\n");
    client_fd = accept(server_fd, NULL, NULL);

    /* Receive file size first */
    recv(client_fd, &filesize_net, sizeof(filesize_net), MSG_WAITALL);
    filesize = ntohl(filesize_net);

    printf("Expecting %u bytes\n", filesize);

    fp = fopen("capture.yuv", "wb");

    while (total < filesize) {
        n = recv(client_fd, buffer, BUF_SIZE, 0);
        if (n <= 0)
            break;
        fwrite(buffer, 1, n, fp);
        total += n;
    }

    printf("Received %zu bytes\n", total);

    fclose(fp);
    close(client_fd);
    close(server_fd);
    return 0;
}

