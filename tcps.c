#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#define PORT 6289
#define BUF_SIZE 1024

int main() {
    int serverSock, clientSock;
    char buffer[BUF_SIZE];
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;

    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSock, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    listen(serverSock, 5);
    printf("Server listening on port %d...\n", PORT);

    addr_size = sizeof(clientAddr);
    clientSock = accept(serverSock, (struct sockaddr *)&clientAddr, &addr_size);
    printf("Client connected.\n");

    while (1) {
        // Receive message from client
        bzero(buffer, BUF_SIZE);
        int bytesReceived = recv(clientSock, buffer, BUF_SIZE, 0);
        if (bytesReceived <= 0) {
            printf("Connection closed by client.\n");
            break;
        }
        buffer[bytesReceived] = '\0'; // Null-terminate the received message
        printf("Client: %s\n", buffer);

        // Count alphabets and digits
        int alphabetCount = 0, digitCount = 0;
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (isalpha(buffer[i])) {
                alphabetCount++;
            } else if (isdigit(buffer[i])) {
                digitCount++;
            }
        }
        printf("Alphabets: %d, Digits: %d\n", alphabetCount, digitCount);

        // Exit if client types bye
        if (strcmp(buffer, "bye") == 0) {
            printf("Client ended the chat.\n");
            break;
        }

        // Send response
        printf("\nServer: ");
        bzero(buffer, BUF_SIZE);
        fgets(buffer, BUF_SIZE, stdin);
        send(clientSock, buffer, strlen(buffer), 0);

        // Exit if server types bye
        if (strncmp(buffer, "bye", 3) == 0) {
            printf("Server ended the chat.\n");
            break;
        }
    }

    close(clientSock);
    close(serverSock);
    return 0;
}
