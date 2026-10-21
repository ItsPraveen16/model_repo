
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int s, c, n, sum = 0, carry, checksum;
    struct sockaddr_in server;
    char msg[100], frame[200], *p;

    s = socket(AF_INET, SOCK_STREAM, 0);

    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    bind(s, (struct sockaddr *)&server, sizeof(server));
    listen(s, 1);

    printf("Checksum receiver waiting...\n");
    c = accept(s, NULL, NULL);

    n = 0;
    while (n < sizeof(frame) - 1) {
        int r = read(c, frame + n, 1);
        if (r <= 0 || frame[n] == '\n')
            break;
        n++;
    }
    frame[n] = '\0';

    p = strrchr(frame, '|');
    if (p == NULL) {
        printf("Invalid data received\n");
        close(c);
        close(s);
        return 1;
    }

    *p = '\0';
    strcpy(msg, frame);
    checksum = atoi(p + 1);

    printf("Received data: %s\n", msg);
    // msg[0] = msg[0] ^ 1;
    // printf("Data corrupted intentionally!\n");      
    printf("Received checksum: %d\n", checksum);

    for (int i = 0; msg[i]; i++) {
        sum += (unsigned char)msg[i];
        carry = sum >> 8;
        sum = (sum & 255) + carry;
        printf("Sum = %d, Carry = %d\n", sum, carry);
    }

    sum += checksum;
    sum = (sum & 255) + (sum >> 8);

    printf("Final verification sum = %d\n", sum);

    if (sum == 255)
        printf("No error detected\n");
    else
        printf("Error detected\n");

    close(c);
    close(s);
    return 0;
}
