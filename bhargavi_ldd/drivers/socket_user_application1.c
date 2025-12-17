#include <stdio.h>

#include <string.h>
#include<stdlib.h>
#include <unistd.h>

#include <arpa/inet.h>
 
#define SERVER_IP "10.58.144.132"

#define SERVER_PORT 5000
 
int main()

{

    int sock;

    struct sockaddr_in server_addr;

    char buffer[128];
 
    /* Create socket */

    sock = socket(AF_INET, SOCK_STREAM, 0);
 
    /* Configure server address */

    server_addr.sin_family = AF_INET;

    server_addr.sin_port = htons(SERVER_PORT);

    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
 
    /* Connect */


if (connect(sock, (struct sockaddr *)&server_addr,
                sizeof(server_addr)) < 0) {
        perror("connect");
        exit(1);
    }
    printf("Connected to server\n");
 
    /* Send data */
    while(1)
	{
    send(sock, "Hello from client", 17, 0);
 
    /* Receive response */

    recv(sock, buffer, sizeof(buffer), 0);

    printf("Server says: %s\n", buffer);
} 
    close(sock);

    return 0;

}



