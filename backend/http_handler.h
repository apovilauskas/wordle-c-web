#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

void serve_file(int socket, const char *path, const char *content_type);
void handle_http_request(int socket);
void handle_new_game(int socket);
void handle_guess(int socket, char *request, int request_size);
void receive_full_body(int socket, char *request, int initial_bytes, char *body, int max_body_len);
void extract_json_value(const char *body, const char *key, char *dest, int max_len);
void send_response(int socket, int status_code, const char *content_type, const char *body);

#endif