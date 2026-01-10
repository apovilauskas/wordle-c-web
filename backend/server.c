#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>        // Replaces winsock specific logic
#include <sys/socket.h>    // Linux socket headers
#include <netinet/in.h>    // Linux address headers
#include "http_handler.h"
#include "game_state.h"

#define PORT 8080

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int opt = 1;
    
    // Initialize game state
    init_game_states();
    
    // Create socket file descriptor
    // AF_INET = IPv4, SOCK_STREAM = TCP
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Forcefully attach socket to the port 8080 (helps with restarts)
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Bind socket to port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen for connections
    if (listen(server_fd, 10) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Server running on port %d\n", PORT);
    
    // Main server loop
    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                                (socklen_t*)&addrlen)) < 0) {
            perror("accept failed");
            continue;
        }
        
        // Handle the request
        handle_http_request(new_socket);
        
        // Close the socket (Linux uses close, not closesocket)
        close(new_socket);
    }
    
    return 0;
}