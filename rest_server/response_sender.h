#ifndef RESPONSE_SENDER_H
#define RESPONSE_SENDER_H

#include "http_server.h"

void send_response(int client_socket, const char *status, const char *body);
void send_json_response(int client_socket, const char *status, const char *json_body);

#endif
