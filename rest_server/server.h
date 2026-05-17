#ifndef SERVER_H
#define SERVER_H

#include "http_server.h"
#include "message_handler.h"

int setup_server_socket(int port);
void start_server(int server_socket);
void stop_server(int server_socket);

#endif