
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);
    char domain[100], ip[100];

    char *names[] = {
        "google.com", "youtube.com", "ssn.edu.in"
    };

    char *ips[] = {
        "142.250.183.14",
        "142.250.72.14",
        "203.0.113.10"
    };

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(sock, (struct sockaddr *)&server, sizeof(server));

    printf("DNS Server running...\n");

    while (1) {
        memset(domain, 0, sizeof(domain));

        recvfrom(sock, domain, sizeof(domain) - 1, 0,
                 (struct sockaddr *)&client, &len);

        domain[strcspn(domain, "\r\n")] = '\0';
        strcpy(ip, "NXDOMAIN");

        for (int i = 0; i < 3; i++) {
            if (strcmp(domain, names[i]) == 0) {
                strcpy(ip, ips[i]);
                break;
            }
        }

        sendto(sock, ip, strlen(ip) + 1, 0,
               (struct sockaddr *)&client, len);

        printf("%s -> %s\n", domain, ip);
    }

    close(sock);
    return 0;
}
