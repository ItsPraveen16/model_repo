
#include <stdio.h>
#include <string.h>

int main() {
    char data[100], gen[100], temp[200];
    int n, m, i, j;

    printf("Enter data bits: ");
    scanf("%99s", data);

    printf("Enter generator bits: ");
    scanf("%99s", gen);

    n = strlen(data);
    m = strlen(gen);

    if (m < 2 || m > n + 1) {
        printf("Invalid generator length\n");
        return 1;
    }

    strcpy(temp, data);

    for (i = 0; i < m - 1; i++)
        temp[n + i] = '0';

    temp[n + m - 1] = '\0';

    for (i = 0; i < n; i++) {
        if (temp[i] == '1') {
            for (j = 0; j < m; j++) {
                temp[i + j] =
                    (temp[i + j] == gen[j]) ? '0' : '1';
            }
        }
    }

    printf("CRC remainder: ");
    for (i = n; i < n + m - 1; i++)
        printf("%c", temp[i]);

    printf("\nCodeword: %s", data);

    for (i = n; i < n + m - 1; i++)
        printf("%c", temp[i]);

    printf("\n");
    return 0;
}
