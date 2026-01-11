#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h> // Linux sockets
#include "http_handler.h"
#include "game_state.h"
#include "logic.h"
#include "words.h"

#define BUFFER_SIZE 32768

// Helper to serve actual files (index.html, style.css, etc.)
void serve_file(int socket, const char* path, const char* content_type) {
    char full_path[512];
    // This looks into the frontend folder relative to where the server runs
    snprintf(full_path, sizeof(full_path), "../frontend%s", path);
    
    FILE* f = fopen(full_path, "rb");
    if (!f) {
        // If file not found in frontend, try root (for index.html at /)
        if (strcmp(path, "/") == 0) {
            f = fopen("../frontend/index.html", "rb");
        }
    }

    if (f) {
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);

        char* content = malloc(fsize + 1);
        fread(content, fsize, 1, f);
        fclose(f);
        content[fsize] = 0;

        send_response(socket, 200, content_type, content);
        free(content);
    } else {
        send_response(socket, 404, "text/plain", "File Not Found");
    }
}

void handle_http_request(int socket) {
    char buffer[BUFFER_SIZE] = {0};
    int bytes_received = recv(socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received <= 0) return;
    
    char method[16], path[256], version[16];
    sscanf(buffer, "%s %s %s", method, path, version);
    
    // 1. Route API Calls
    if (strcmp(method, "POST") == 0 && strcmp(path, "/api/new-game") == 0) {
        handle_new_game(socket);
    } 
    else if (strcmp(method, "POST") == 0 && strcmp(path, "/api/guess") == 0) {
        handle_guess(socket, buffer);
    }
    // 2. Handle CORS Preflight
    else if (strcmp(method, "OPTIONS") == 0) {
        char* response = "HTTP/1.1 204 No Content\r\n"
                         "Access-Control-Allow-Origin: *\r\n"
                         "Access-Control-Allow-Methods: POST, GET, OPTIONS\r\n"
                         "Access-Control-Allow-Headers: Content-Type\r\n"
                         "\r\n";
        send(socket, response, strlen(response), 0);
    }
    // 3. Serve Static Frontend Files
    else if (strcmp(path, "/") == 0 || strcmp(path, "/index.html") == 0) {
        serve_file(socket, "/index.html", "text/html");
    } 
    else if (strstr(path, ".css")) {
        serve_file(socket, path, "text/css");
    } 
    else if (strstr(path, ".js")) {
        serve_file(socket, path, "application/javascript");
    }
    else {
        send_response(socket, 404, "application/json", "{\"error\":\"Not found\"}");
    }
}

void handle_new_game(int socket) {
    char* session_id = create_new_game();
    
    char response[256];
    snprintf(response, sizeof(response), 
             "{\"sessionId\":\"%s\",\"message\":\"New game started\"}", 
             session_id);
    
    send_response(socket, 200, "application/json", response);
}

#include <ctype.h> // Needed for toupper

// Helper to extract value cleanly (stops at the closing quote)
void extract_json_value(const char* body, const char* key, char* dest, int max_len) {
    char search_key[64];
    snprintf(search_key, sizeof(search_key), "\"%s\":\"", key); // Looks for "key":"
    
    char* start = strstr(body, search_key);
    if (start) {
        start += strlen(search_key); // Move past "key":"
        int i = 0;
        // Copy until we hit the closing quote " or end of buffer
        while (start[i] != '"' && start[i] != '\0' && i < max_len - 1) {
            dest[i] = start[i];
            i++;
        }
        dest[i] = '\0'; // Null-terminate cleanly
    }
}

void handle_guess(int socket, char* request) {
    // 1. Extract Body
    char* body = strstr(request, "\r\n\r\n");
    if (!body) {
        send_response(socket, 400, "application/json", "{\"error\":\"Invalid request\"}");
        return;
    }
    body += 4; 

    // 2. Parse JSON cleanly
    char guess[10] = {0};       // slightly larger buffer for safety
    char session_id[64] = {0};  // slightly larger buffer
    
    extract_json_value(body, "guess", guess, sizeof(guess));
    extract_json_value(body, "sessionId", session_id, sizeof(session_id));

    printf("DEBUG: Parsed Guess: '[%s]', Session: '[%s]'\n", guess, session_id);

    // 3. Force Guess to Uppercase
    for(int i=0; guess[i]; i++){
        guess[i] = toupper((unsigned char)guess[i]);
    }

    // 4. Validate Word
    if (strlen(guess) != 5) {
        send_response(socket, 400, "application/json", "{\"error\":\"Guess must be 5 letters\"}");
        return;
    }

    if (!isValidGuess(guess)) {
        printf("DEBUG: Word '%s' not found in dictionary.\n", guess);
        send_response(socket, 400, "application/json", "{\"error\":\"Invalid word\"}");
        return;
    }

    // 5. Check Session
    char* secret_word = get_secret_word(session_id);
    if (secret_word == NULL) {
        printf("DEBUG: Session '%s' not found!\n", session_id);
        send_response(socket, 404, "application/json", "{\"error\":\"Session not found\"}");
        return;
    }

    // 6. Evaluate
    int* result = evaluateGuess(secret_word, guess);
    char* json_result = createJsonResult(result);
    
    send_response(socket, 200, "application/json", json_result);
    
    free(result);
    free(json_result);
}

void send_response(int socket, int status_code, const char* content_type, const char* body) {
    char header[4096];
    int body_len = body ? strlen(body) : 0;

    snprintf(header, sizeof(header),
        "HTTP/1.1 %d OK\r\n"
        "Content-Type: %s\r\n"
        "Access-Control-Allow-Origin: *\r\n"
        "Access-Control-Allow-Methods: GET, POST, OPTIONS\r\n"
        "Access-Control-Allow-Headers: Content-Type\r\n"
        "Content-Length: %d\r\n"
        "\r\n",
        status_code, content_type, body_len
    );

    send(socket, header, strlen(header), 0);
    if (body_len > 0) {
        send(socket, body, body_len, 0);
    }
}
