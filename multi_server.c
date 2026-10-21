
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>

int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    char buffer[1024];

    signal(SIGCHLD, SIG_IGN);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(8080);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 5);

    printf("Multiclient server waiting...\n");

    while (1) {
        client_fd = accept(server_fd, NULL, NULL);

        if (client_fd < 0)
            continue;

        pid_t pid = fork();

        if (pid == 0) {
            // Child process
            close(server_fd);

            memset(buffer, 0, sizeof(buffer));
            read(client_fd, buffer, sizeof(buffer) - 1);

            printf("Client says: %s\n", buffer);
            write(client_fd, "Message received", 16);

            close(client_fd);
            return 0;
        } else if (pid > 0) {
            // Parent process
            close(client_fd);
        } else {
            close(client_fd);
        }
    }

    close(server_fd);
    return 0;
}
