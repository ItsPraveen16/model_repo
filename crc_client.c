
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void divide(char *data, int n) {
    char gen[] = "10011";
    int i, j;

    for (i = 0; i <= n - 5; i++) {
        if (data[i] == '1') {
            for (j = 0; j < 5; j++)
                data[i+j] =
                    (data[i+j] == gen[j]) ? '0' : '1';
        }
    }
}

int main() {
    int s, i, n;
    char msg[20], data[200], temp[200];
    struct sockaddr_in server;

    printf("Enter data (max 19 characters): ");
    scanf("%19s", msg);

    // Convert characters to binary
    for (i = 0; msg[i]; i++) {
        int j;
        for (j = 7; j >= 0; j--)
            data[i*8 + (7-j)] = ((msg[i] >> j) & 1) + '0';
    }

    n = strlen(msg) * 8;
    data[n] = '\0';

    // Append four zeros
    strcat(data, "0000");
    strcpy(temp, data);

    divide(temp, strlen(temp));

    // Append the remainder to original data
    strcpy(data + n, temp + strlen(temp) - 4);

    printf("CRC frame: %s\n", data);

    // Introduce an error if requested
    int error;
    printf("Introduce bit error? (1=yes, 0=no): ");
    scanf("%d", &error);

    if (error == 1)
        data[0] = (data[0] == '0') ? '1' : '0';

    s = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    connect(s, (struct sockaddr *)&server, sizeof(server));
    write(s, data, strlen(data));

    printf("Frame sent!\n");
    close(s);
    return 0;
}
