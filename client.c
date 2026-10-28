
// #include <stdio.h>
// #include <string.h>
// #include <unistd.h>
// #include <arpa/inet.h>

// int main() {
//     int sock;
//     struct sockaddr_in server_addr;
//     char *msg = "Hello Server";

//     sock = socket(AF_INET, SOCK_STREAM, 0);

//     server_addr.sin_family = AF_INET;
//     server_addr.sin_port = htons(8080);
//     inet_pton(AF_INET, "127.0.0.1",
//               &server_addr.sin_addr);

//     connect(sock, (struct sockaddr *)&server_addr,
//             sizeof(server_addr));

//     write(sock, msg, strlen(msg));

//     close(sock);
//     return 0;
// }


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char *msg = "Hello Server";
    char buffer[1024] = {0};

    sock = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1",
              &server_addr.sin_addr);

    connect(sock, (struct sockaddr *)&server_addr,
            sizeof(server_addr));

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    write(sock, msg, strlen(msg));

    int n = read(sock, buffer, sizeof(buffer) - 1);

    clock_gettime(CLOCK_MONOTONIC, &end);

    if (n > 0) {
        buffer[n] = '\0';
        printf("Server says: %s\n", buffer);
    }

    double time_ms = (end.tv_sec - start.tv_sec) * 1000.0
                   + (end.tv_nsec - start.tv_nsec) / 1000000.0;

    printf("Message size: %zu bytes\n", strlen(msg));
    printf("Round Trip Time: %.3f ms\n", time_ms);

    if (time_ms > 0) {
        printf("Approx. request rate: %.3f kbps\n",
               strlen(msg) * 8.0 / time_ms);
    }

    close(sock);
    return 0;
}
