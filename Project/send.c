#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/stat.h>
 
#define SERVER_IP   "10.58.144.99"
#define SERVER_PORT 5000
#define BUF_SIZE    4096
 
int main(void)
{
    int sock;
    struct sockaddr_in server;
    FILE *fp;
    char buffer[BUF_SIZE];
    size_t n;
    struct stat st;
    uint32_t filesize_net;
 
    stat("capture.yuv", &st);
    uint32_t filesize = st.st_size;
 
    sock = socket(AF_INET, SOCK_STREAM, 0);
 
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server.sin_addr);
 
    connect(sock, (struct sockaddr *)&server, sizeof(server));
 
    /* Send file size first */
    filesize_net = htonl(filesize);
    send(sock, &filesize_net, sizeof(filesize_net), 0);
 
    fp = fopen("capture.yuv", "rb");
 
    while ((n = fread(buffer, 1, BUF_SIZE, fp)) > 0) {
        send(sock, buffer, n, 0);
    }
 
    printf("Sent %u bytes\n", filesize);
 
    fclose(fp);
    close(sock);
    return 0;
}
