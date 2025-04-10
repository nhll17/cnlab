#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main() {
    int client;
    struct sockaddr_in servAddr;
    char servMsg[2000], cliMsg[2000];
    socklen_t server_struct_length = sizeof(servAddr);

    client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(2002);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("UDP Chat Client Started\n");

    while (1) {
        // Send message
        printf("You: ");
        fgets(cliMsg, sizeof(cliMsg), stdin);
        cliMsg[strcspn(cliMsg, "\n")] = 0;

        sendto(client, cliMsg, strlen(cliMsg), 0, (struct sockaddr *)&servAddr, server_struct_length);

        // Exit if client says bye
        if (strcmp(cliMsg, "bye") == 0) {
            printf("You ended the chat.\n");
            break;
        }

        // Receive server message
        recvfrom(client, servMsg, sizeof(servMsg), 0, (struct sockaddr *)&servAddr, &server_struct_length);
        servMsg[strcspn(servMsg, "\n")] = 0;

        printf("Server: %s\n", servMsg);

        // Exit if server says bye
        if (strcmp(servMsg, "bye") == 0) {
            printf("Server ended the chat.\n");
            break;
        }
    }

    close(client);
    return 0;
}
