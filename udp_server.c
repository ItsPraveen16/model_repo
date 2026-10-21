
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len = sizeof(client_addr);
    char buffer[1024] = {0};

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    bind(sock, (struct sockaddr *)&server_addr,
         sizeof(server_addr));

    printf("UDP Server waiting...\n");

    recvfrom(sock, buffer, sizeof(buffer) - 1, 0,
             (struct sockaddr *)&client_addr, &len);

    printf("Client says: %s\n", buffer);

    sendto(sock, "Message received", 16, 0,
           (struct sockaddr *)&client_addr, len);

    close(sock);
    return 0;
}
