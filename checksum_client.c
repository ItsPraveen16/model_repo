
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s, sum = 0, carry, checksum;
    struct sockaddr_in server;
    char msg[100], frame[200];

    printf("Enter data: ");
    scanf("%99s", msg);

    // Calculate checksum
    for (int i = 0; msg[i]; i++) {
        sum += (unsigned char)msg[i];
        carry = sum >> 8;
        sum = (sum & 255) + carry;

        printf("Sum = %d, Carry = %d\n", sum, carry);
    }

    checksum = (~sum) & 255;

    printf("Checksum = %d\n", checksum);

    // Create TCP socket
    s = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(s, (struct sockaddr *)&server,
                sizeof(server)) < 0) {
        perror("connect");
        close(s);
        return 1;
    }

    // Send data and checksum
    snprintf(frame, sizeof(frame), "%s|%d\n", msg, checksum);
    write(s, frame, strlen(frame));

    printf("Data and checksum sent!\n");

    close(s);
    return 0;
}
