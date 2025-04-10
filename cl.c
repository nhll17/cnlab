#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#define PORT 6289
#define BUF_SIZE 1024

int main() {
    int clientSock;
    char buffer[BUF_SIZE];
    struct sockaddr_in serverAddr;

    clientSock = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(clientSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    printf("Connected to the server.\n");

    while (1) {
        // Send message to server
        printf("Client: ");
        bzero(buffer, BUF_SIZE);
        fgets(buffer, BUF_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0'; // Remove newline character
        send(clientSock, buffer, strlen(buffer), 0);

        // Exit if client types bye
        if (strcmp(buffer, "bye") == 0) {
            printf("Client ended the chat.\n");
            break;
        }

        // Receive server response
        bzero(buffer, BUF_SIZE);
        recv(clientSock, buffer, BUF_SIZE, 0);
        printf("Server: %s", buffer);

        // Exit if server types bye
        if (strncmp(buffer, "bye", 3) == 0) {
            printf("Server ended the chat.\n");
            break;
        }
    }

    close(clientSock);
    return 0;
}