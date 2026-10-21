
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock;
    struct sockaddr_in server_addr;
    socklen_t len = sizeof(server_addr);
    char buffer[1024] = {0};
    char *msg = "Hello UDP Server";

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1",
              &server_addr.sin_addr);

    sendto(sock, msg, strlen(msg), 0,
           (struct sockaddr *)&server_addr,
           sizeof(server_addr));

    recvfrom(sock, buffer, sizeof(buffer) - 1, 0,
             (struct sockaddr *)&server_addr, &len);

    printf("Server says: %s\n", buffer);

    close(sock);
    return 0;
}
