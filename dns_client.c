
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <ctype.h>

int main() {
    int sock;
    struct sockaddr_in server;
    struct timeval tv;
    char domain[100], ip[100];
    char cache_name[10][100], cache_ip[10][100];
    int count = 0, found, i;
    socklen_t len = sizeof(server);

    sock = socket(AF_INET, SOCK_DGRAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Timeout: 3 seconds
    tv.tv_sec = 3;
    tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

    while (1) {
        printf("\nEnter domain (exit to stop): ");
        scanf("%99s", domain);

        if (strcmp(domain, "exit") == 0)
            break;

        // Validate domain format
        found = 0;
        if (strchr(domain, '.') == NULL ||
            strchr(domain, ' ') != NULL) {
            printf("Invalid domain format!\n");
            continue;
        }

        for (i = 0; domain[i]; i++)
            domain[i] = tolower((unsigned char)domain[i]);

        // Check cache
        for (i = 0; i < count; i++) {
            if (strcmp(domain, cache_name[i]) == 0) {
                printf("Cache hit: %s\n", cache_ip[i]);
                found = 1;
                break;
            }
        }

        if (found)
            continue;

        // Send UDP query
        sendto(sock, domain, strlen(domain) + 1, 0,
               (struct sockaddr *)&server, len);

        // Receive reply
        memset(ip, 0, sizeof(ip));

        if (recvfrom(sock, ip, sizeof(ip) - 1, 0,
                     (struct sockaddr *)&server, &len) < 0) {
            printf("DNS request timed out!\n");
            continue;
        }

        printf("DNS Response: %s\n", ip);

        // Cache successful results only
        if (strcmp(ip, "NXDOMAIN") != 0 && count < 10) {
            strcpy(cache_name[count], domain);
            strcpy(cache_ip[count], ip);
            count++;
        }
    }

    close(sock);
    return 0;
}
