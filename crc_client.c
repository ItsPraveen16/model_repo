
// #include <stdio.h>
// #include <string.h>
// #include <unistd.h>
// #include <arpa/inet.h>

// void divide(char *data, int n) {
//     char gen[] = "10011";
//     int i, j;

//     for (i = 0; i <= n - 5; i++) {
//         if (data[i] == '1') {
//             for (j = 0; j < 5; j++)
//                 data[i+j] =
//                     (data[i+j] == gen[j]) ? '0' : '1';
//         }
//     }
// }

// int main() {
//     int s, i, n;
//     char msg[20], data[200], temp[200];
//     struct sockaddr_in server;

//     printf("Enter data (max 19 characters): ");
//     scanf("%19s", msg);

//     // Convert characters to binary
//     for (i = 0; msg[i]; i++) {
//         int j;
//         for (j = 7; j >= 0; j--)
//             data[i*8 + (7-j)] = ((msg[i] >> j) & 1) + '0';
//     }

//     n = strlen(msg) * 8;
//     data[n] = '\0';

//     // Append four zeros
//     strcat(data, "0000");
//     strcpy(temp, data);

//     divide(temp, strlen(temp));

//     // Append the remainder to original data
//     strcpy(data + n, temp + strlen(temp) - 4);

//     printf("CRC frame: %s\n", data);

//     // Introduce an error if requested
//     int error;
//     printf("Introduce bit error? (1=yes, 0=no): ");
//     scanf("%d", &error);

//     if (error == 1)
//         data[0] = (data[0] == '0') ? '1' : '0';

//     s = socket(AF_INET, SOCK_STREAM, 0);

//     server.sin_family = AF_INET;
//     server.sin_port = htons(8080);
//     server.sin_addr.s_addr = inet_addr("127.0.0.1");

//     connect(s, (struct sockaddr *)&server, sizeof(server));
//     write(s, data, strlen(data));

//     printf("Frame sent!\n");
//     close(s);
//     return 0;
// }


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define GEN "10011"

void divide(char *data, int n) {
    int g = strlen(GEN), i, j;

    for (i = 0; i <= n - g; i++) {
        if (data[i] == '1') {
            for (j = 0; j < g; j++)
                data[i+j] =
                    (data[i+j] == GEN[j]) ? '0' : '1';
        }
    }
}

int main() {
    int s, i, n, g = strlen(GEN);
    char msg[20], data[200], temp[200];
    struct sockaddr_in server;
    struct timespec start, end;

    printf("Enter data (max 19 characters): ");
    scanf("%19s", msg);

    clock_gettime(CLOCK_MONOTONIC, &start);

    // Convert characters to binary
    for (i = 0; msg[i]; i++) {
        int j;
        for (j = 7; j >= 0; j--)
            data[i*8 + (7-j)] = ((msg[i] >> j) & 1) + '0';
    }

    n = strlen(msg) * 8;
    data[n] = '\0';

    // Append generator length - 1 zeros
    for (i = 0; i < g - 1; i++)
        data[n+i] = '0';
    data[n+g-1] = '\0';

    strcpy(temp, data);
    divide(temp, strlen(temp));

    // Append CRC remainder to original data
    for (i = 0; i < g - 1; i++)
        data[n+i] = temp[n+i];

    data[n+g-1] = '\0';

    clock_gettime(CLOCK_MONOTONIC, &end);

    double encode_ms =
        (end.tv_sec - start.tv_sec) * 1000.0 +
        (end.tv_nsec - start.tv_nsec) / 1000000.0;

    printf("CRC frame: %s\n", data);
    printf("Generator: %s\n", GEN);
    printf("Frame size: %zu bits\n", strlen(data));
    printf("CRC encoding time: %.3f ms\n", encode_ms);

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

    clock_gettime(CLOCK_MONOTONIC, &start);
    write(s, data, strlen(data));
    clock_gettime(CLOCK_MONOTONIC, &end);

    double send_ms =
        (end.tv_sec - start.tv_sec) * 1000.0 +
        (end.tv_nsec - start.tv_nsec) / 1000000.0;

    printf("Send call time: %.3f ms\n", send_ms);
    printf("Frame sent!\n");

    close(s);
    return 0;
}
