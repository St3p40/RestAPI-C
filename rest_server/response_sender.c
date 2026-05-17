#include "response_sender.h"

void send_response(int client_socket, const char *status, const char *body) {
    char response[2048];
    snprintf(response, sizeof(response),
             "HTTP/1.1 %s\r\nContent-Type: text/plain\r\nContent-Length: %lu\r\n\r\n%s",
             status, strlen(body), body);
    send(client_socket, response, strlen(response), 0);
}

void send_json_response(int client_socket, const char *status, const char *json_body) {
    char response[2048];
    snprintf(response, sizeof(response),
             "HTTP/1.1 %s\r\nContent-Type: application/json\r\nContent-Length: %lu\r\n\r\n%s",
             status, strlen(json_body), json_body);
    send(client_socket, response, strlen(response), 0);
}
