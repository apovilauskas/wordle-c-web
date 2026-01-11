#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>        // Replaces winsock specific logic
#include <sys/socket.h>    // Linux socket headers
#include <netinet/in.h>    // Linux address headers
#include "http_handler.h"
#include "game_state.h"
#include "words.h"
#include <time.h>





int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int opt = 1;

    srand(time(NULL));
    loadDictionary("words.txt");
    init_game_states();

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    int port = 8080;
    char *env_port = getenv("PORT");
    if (env_port) port = atoi(env_port);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    listen(server_fd, 10);

    printf("Server running on port %d\n", port);

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        handle_http_request(new_socket);
        close(new_socket);
    }
}
