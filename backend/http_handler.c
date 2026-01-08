#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <winsock2.h>
#include <ctype.h>
#include <ws2tcpip.h>
#include "http_handler.h"
#include "game_state.h"
#include "logic.h"
#include "words.h"
#define BUFFER_SIZE 32768
// Forward declarations
void handle_new_game(int socket);
void handle_guess(int socket, char* request);



void send_cors_headers(int socket) {
    char* cors = "Access-Control-Allow-Origin: *\r\n"
                 "Access-Control-Allow-Methods: POST, GET, OPTIONS\r\n"
                 "Access-Control-Allow-Headers: Content-Type\r\n";
    send(socket, cors, strlen(cors), 0);
}

void send_response(int socket, int status_code, const char* content_type, const char* body) {
    char header[1024];
    
    // 1. Build only the header
    int header_len = snprintf(header, sizeof(header),
         "HTTP/1.1 %d OK\r\n"
         "Content-Type: %s\r\n"
         "Content-Length: %zu\r\n"
         "Access-Control-Allow-Origin: *\r\n" // Put CORS here directly
         "Access-Control-Allow-Methods: POST, GET, OPTIONS\r\n"
         "Access-Control-Allow-Headers: Content-Type\r\n"
         "\r\n", 
         status_code, content_type, strlen(body));

    // 2. Send the header
    send(socket, header, header_len, 0);
    
    // 3. Send the body once
    send(socket, body, strlen(body), 0);
}

void handle_http_request(int socket) {
    char buffer[BUFFER_SIZE] = {0};
    recv(socket, buffer, BUFFER_SIZE, 0);
    
    // Parse request line
    char method[16], path[256], version[16];
    sscanf(buffer, "%s %s %s", method, path, version);
    
    printf("Request: %s %s\n", method, path);
    
    // Handle OPTIONS (CORS preflight)
    if (strcmp(method, "OPTIONS") == 0) {
        char* response = "HTTP/1.1 204 No Content\r\n";
        send(socket, response, strlen(response), 0);
        send_cors_headers(socket);
        send(socket, "\r\n", 2, 0);
        return;
    }
    
    // Route requests
    if (strcmp(method, "POST") == 0 && strcmp(path, "/api/new-game") == 0) {
        handle_new_game(socket);
    } 
    else if (strcmp(method, "POST") == 0 && strcmp(path, "/api/guess") == 0) {
        handle_guess(socket, buffer);
    }else if (strcmp(method, "GET") == 0 && strcmp(path, "/") == 0) {
        send_response(socket, 200, "text/html", "<h1>Wordle API is Running</h1>");
        return;
    } else {
        char* error = "{\"error\":\"Not found\"}";
        send_response(socket, 404, "application/json", error);
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