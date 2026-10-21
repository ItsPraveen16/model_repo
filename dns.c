
#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>

int main() {
    char hostname[100];
    struct hostent *host;
    struct in_addr **addr_list;

    printf("Enter hostname: ");
    scanf("%99s", hostname);

    host = gethostbyname(hostname);

    if (host == NULL) {
        printf("DNS lookup failed\n");
        return 1;
    }

    addr_list = (struct in_addr **)host->h_addr_list;

    printf("IP address: %s\n",
           inet_ntoa(*addr_list[0]));

    return 0;
}
