#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    int server;
    struct sockaddr_in servAddr, client_addr;
    char servMsg[2000], cliMsg[2000];
    socklen_t client_struct_length = sizeof(client_addr);

    server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(2002);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(server, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    printf("UDP Server is up. Waiting for messages...\n");

    while (1) {
        // Receive message from client
        recvfrom(server, cliMsg, sizeof(cliMsg), 0, (struct sockaddr *)&client_addr, &client_struct_length);
        cliMsg[strcspn(cliMsg, "\n")] = 0;
        printf("Client: %s\n", cliMsg);

        // Exit if client says bye
        if (strcmp(cliMsg, "bye") == 0) {
            printf("Client ended the chat.\n");
            break;
        }

        // Get response from server
        printf("You: ");
        fgets(servMsg, sizeof(servMsg), stdin);
        servMsg[strcspn(servMsg, "\n")] = 0;

        sendto(server, servMsg, strlen(servMsg), 0, (struct sockaddr *)&client_addr, client_struct_length);

        // Exit if server says bye
        if (strcmp(servMsg, "bye") == 0) {
            printf("You ended the chat.\n");
            break;
        }
    }

    close(server);
    return 0;
}
