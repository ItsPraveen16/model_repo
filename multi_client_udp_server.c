
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sockfd;
    char buffer[1024];
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(5000);

    if (bind(sockfd, (struct sockaddr *)&server,
             sizeof(server)) < 0) {
        perror("Bind failed");
        close(sockfd);
        return 1;
    }

    printf("UDP server waiting for clients...\n");

    while (1) {
        int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                         (struct sockaddr *)&client, &len);

        if (n < 0) {
            perror("Receive failed");
            continue;
        }

        buffer[n] = '\0';

        printf("Client %s:%d says: %s\n",
               inet_ntoa(client.sin_addr),
               ntohs(client.sin_port), buffer);

        sendto(sockfd, buffer, n, 0,
               (struct sockaddr *)&client, len);
    }

    close(sockfd);
    return 0;
}
