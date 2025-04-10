#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include<ctype.h>
#define PORT 6276
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
        
        bzero(buffer, BUF_SIZE);
        recv(clientSock, buffer, BUF_SIZE, 0);
        buffer[strcspn(buffer, "\n")] = 0;
        printf("Client: %s", buffer);

        // Exit if client types bye
       if (strcmp(buffer, "bye") == 0)
 {
            printf("Client ended the chat.\n");
            break;
        }
        int alphabetCount = 0, digitCount = 0;
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (isalpha(buffer[i])) {
                alphabetCount++;
            } else if (isdigit(buffer[i])) {
                digitCount++;
            }
        }
        // Send response
        printf("\n Server: ");
        bzero(buffer, BUF_SIZE);
       // fgets(buffer, BUF_SIZE, stdin);
       snprintf(buffer, BUF_SIZE, "alphabets %d, digits %d", alphabetCount, digitCount);
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
