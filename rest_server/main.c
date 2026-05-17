#include "http_server.h"

int main() {
    int server_socket = setup_server_socket(8080);

    start_server(server_socket);

    stop_server(server_socket);

    return 0;
}