#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

void handle_http_request(int socket);
void send_response(int socket, int status_code, const char* content_type, const char* body);

#endif