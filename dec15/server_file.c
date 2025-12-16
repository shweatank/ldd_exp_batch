#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PORT 8080
#define BUF_SIZE 1024
#define FILENAME "test.txt"

int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    char buffer[BUF_SIZE];
    int file_fd;
    struct stat st;
    ssize_t n;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 1);

    printf("Waiting for client...\n");
    client_fd = accept(server_fd, NULL, NULL);
    printf("Client connected\n");

    /* Open file */
    file_fd = open(FILENAME, O_RDONLY);
    if (file_fd < 0) {
        perror("File open failed");
        close(client_fd);
        return 1;
    }

    /* Get and send file size */
    fstat(file_fd, &st);
    long filesize = st.st_size;
    write(client_fd, &filesize, sizeof(filesize));

    /* Send file content */
    while ((n = read(file_fd, buffer, BUF_SIZE)) > 0) {
        write(client_fd, buffer, n);
    }

    printf("File sent successfully\n");

    close(file_fd);
    close(client_fd);
    close(server_fd);
    return 0;
}

