#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include "data_store.h"
#include "response_sender.h"

#define HTTP_OK "200 OK"
#define HTTP_CREATED "201 Created"
#define HTTP_BAD_REQUEST "400 Bad Request"
#define HTTP_NOT_FOUND "404 Not Found"
#define HTTP_METHOD_NOT_ALLOWED "405 Method Not Allowed"
#define HTTP_INTERNAL_SERVER_ERROR "500 Internal Server Error"

#define BUFFER_SIZE 2048

void handle_client_request(int client_socket);

void handle_get_request(int client_socket, const char *path);
void handle_post_request(int client_socket, const char *path, const char *body);
void handle_put_request(int client_socket, const char *path, const char *body);
void handle_patch_request(int client_socket, const char *path, const char *body);
void handle_delete_request(int client_socket, const char *path);

#endif
