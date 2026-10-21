
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

int main() {
    int sock;
    struct sockaddr_in server;
    char request[1024], buffer[4096];
    char filename[100];
    int n, header = 0;
    FILE *fp;
    long size = 0;
    clock_t start, end;

    printf("Enter filename: ");
    scanf("%99s", filename);

    start = clock();

    // Create TCP socket
    sock = socket(AF_INET, SOCK_STREAM, 0);

    // Server address
    server.sin_family = AF_INET;
    server.sin_port = htons(8000);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    connect(sock, (struct sockaddr *)&server,
            sizeof(server));

    // Send HTTP GET request
    sprintf(request,
            "GET /%s HTTP/1.1\r\n"
            "Host: localhost\r\n"
            "Connection: close\r\n\r\n", filename);

    send(sock, request, strlen(request), 0);

    // Save response body
    fp = fopen("downloaded_file", "wb");

    while ((n = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        int i = 0;

        if (!header) {
            while (i < n) {
                if (i + 3 < n &&
                    buffer[i] == '\r' &&
                    buffer[i+1] == '\n' &&
                    buffer[i+2] == '\r' &&
                    buffer[i+3] == '\n') {
                    i += 4;
                    header = 1;
                    break;
                }
                i++;
            }
        }

        if (header && i < n) {
            fwrite(buffer + i, 1, n - i, fp);
            size += n - i;
        } else if (header) {
            fwrite(buffer, 1, n, fp);
            size += n;
        }
    }

    fclose(fp);
    close(sock);

    end = clock();

    printf("File downloaded successfully!\n");
    printf("File size: %ld bytes\n", size);
    printf("Time: %.3f seconds\n",
           (double)(end - start) / CLOCKS_PER_SEC);

    if (end > start)
        printf("Throughput: %.2f KB/s\n",
               size / 1024.0 /
               (((double)(end - start)) / CLOCKS_PER_SEC));

    return 0;
}
