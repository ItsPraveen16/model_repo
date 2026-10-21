
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void divide(char *data, int n) {
    char gen[] = "10011";
    int i, j;

    for (i = 0; i <= n - 5; i++) {
        if (data[i] == '1') {
            for (j = 0; j < 5; j++) {
                data[i+j] =
                    (data[i+j] == gen[j]) ? '0' : '1';
            }
        }
    }
}

int main() {
    int s, c, n, i, error = 0;
    struct sockaddr_in server;
    char frame[200], temp[200];

    // Create TCP socket
    s = socket(AF_INET, SOCK_STREAM, 0);

    // Server address
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    // Bind and listen
    bind(s, (struct sockaddr *)&server, sizeof(server));
    listen(s, 1);

    printf("CRC Receiver waiting...\n");

    // Accept client
    c = accept(s, NULL, NULL);

    // Receive CRC frame
    n = read(c, frame, sizeof(frame) - 1);

    if (n <= 0) {
        printf("No data received\n");
        close(c);
        close(s);
        return 1;
    }

    frame[n] = '\0';

    printf("Received frame: %s\n", frame);

    // Perform CRC division
    strcpy(temp, frame);
    divide(temp, strlen(temp));

    // Check final 4 remainder bits
    printf("CRC remainder: ");
    for (i = strlen(temp) - 4; i < strlen(temp); i++) {
        printf("%c", temp[i]);

        if (temp[i] != '0')
            error = 1;
    }

    printf("\n");

    if (error == 0)
        printf("Frame is error-free\n");
    else
        printf("Error detected in received frame\n");

    close(c);
    close(s);

    return 0;
}
