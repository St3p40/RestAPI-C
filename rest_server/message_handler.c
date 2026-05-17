#include "message_handler.h"

void handle_client_request(int client_socket) {
    char buffer[BUFFER_SIZE];
    int received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (received <= 0) {
        close(client_socket);
        return;
    }

    buffer[received] = '\0';

    char method[8], path[256], version[16];
    sscanf(buffer, "%s %s %s", method, path, version);

    if (strcmp(method, "GET") == 0) {
        handle_get_request(client_socket, path);
    } else if (strcmp(method, "POST") == 0) {
        char *body = strstr(buffer, "\r\n\r\n");
        if (body) {
            body += 4;
            handle_post_request(client_socket, path, body);
        } else {
            send_response(client_socket, HTTP_BAD_REQUEST, "{\"error\": \"Bad Request\"}");
        }
    } else if (strcmp(method, "PUT") == 0) {
        char *body = strstr(buffer, "\r\n\r\n");
        if (body) {
            body += 4;
            handle_put_request(client_socket, path, body);
        } else {
            send_response(client_socket, HTTP_BAD_REQUEST, "{\"error\": \"Bad Request\"}");
        }
    } else if (strcmp(method, "PATCH") == 0) {
        char *body = strstr(buffer, "\r\n\r\n");
        if (body) {
            body += 4;
            handle_patch_request(client_socket, path, body);
        } else {
            send_response(client_socket, HTTP_BAD_REQUEST, "{\"error\": \"Bad Request\"}");
        }
    } else if (strcmp(method, "DELETE") == 0) {
        handle_delete_request(client_socket, path);
    } else {
        send_response(client_socket, HTTP_METHOD_NOT_ALLOWED, "{\"error\": \"Method Not Allowed\"}");
    }

    close(client_socket);
}

void handle_get_request(int client_socket, const char *path) {
    int id;
    if (sscanf(path, "/items/%d", &id) == 1) {

        Item *item = get_item(id);
        if (item) {
            char body[1024];
            snprintf(body, sizeof(body), "{\"id\": %d, \"name\": \"%s\", \"description\": \"%s\", \"price\": %.2f}",
                     item->id, item->name, item->description, item->price);
            send_json_response(client_socket, HTTP_OK, body);
        } else {
            send_response(client_socket, HTTP_BAD_REQUEST, "{\"error\": \"Item not found\"}");
        }
    } else if ((strcmp(path, "/items") == 0) || (strcmp(path, "/items/") == 0)) {

        char response_body[4096];
        int offset = 0;
        offset += snprintf(response_body + offset, sizeof(response_body) - offset, "[");

        for (int i = 0; i < get_item_count(); i++) {
            if (i > 0) {
                offset += snprintf(response_body + offset, sizeof(response_body) - offset, ",");
            }
            Item *item = get_item(i + 1);
            if (!item){
                send_response(client_socket, HTTP_INTERNAL_SERVER_ERROR, "{\"error\": \"Internal server error\"}");
                return;
            }
            offset += snprintf(response_body + offset, sizeof(response_body) - offset,
                                "{\"id\": %d, \"name\": \"%s\", \"description\": \"%s\", \"price\": %.2f}",
                                item->id, item->name, item->description, item->price);
        }

        offset += snprintf(response_body + offset, sizeof(response_body) - offset, "]");
        send_json_response(client_socket, HTTP_OK, response_body);
    } else {
        send_response(client_socket, HTTP_BAD_REQUEST, "{\"error\": \"Invalid request\"}");
    }
}

void handle_post_request(int client_socket, const char *path, const char *body) {
    cJSON *json = cJSON_Parse(body);
    if (json == NULL) {
        send_response(client_socket, HTTP_BAD_REQUEST, "{\"error\": \"Invalid JSON\"}");
        return;
    }

    cJSON *name_item = cJSON_GetObjectItemCaseSensitive(json, "name");
    cJSON *description_item = cJSON_GetObjectItemCaseSensitive(json, "description");
    cJSON *price_item = cJSON_GetObjectItemCaseSensitive(json, "price");

    if (!cJSON_IsString(name_item) || !cJSON_IsString(description_item) || !cJSON_IsNumber(price_item)) {
        send_response(client_socket, HTTP_BAD_REQUEST, "{\"error\": \"Missing or invalid fields\"}");
        cJSON_Delete(json);
        return;
    }

    const char *name = name_item->valuestring;
    const char *description = description_item->valuestring;
    float price = (float)price_item->valuedouble;

    int id = create_item(name, description, price);
    if (id > 0) {
        char response_body[1024];
        snprintf(response_body, sizeof(response_body), "{\"message\": \"Item created\", \"id\": %d}", id);
        send_json_response(client_socket, HTTP_OK, response_body);
    } else {
        send_response(client_socket, HTTP_INTERNAL_SERVER_ERROR, "{\"error\": \"Could not create item\"}");
    }

    cJSON_Delete(json);
}

void handle_put_request(int client_socket, const char *path, const char *body) {
    int id;
    if (sscanf(path, "/items/%d", &id) == 1) {

        cJSON *json = cJSON_Parse(body);
        if (json == NULL) {
            send_response(client_socket, HTTP_BAD_REQUEST, "{\"error\": \"Invalid JSON\"}");
            return;
        }

        cJSON *name_item = cJSON_GetObjectItemCaseSensitive(json, "name");
        cJSON *description_item = cJSON_GetObjectItemCaseSensitive(json, "description");
        cJSON *price_item = cJSON_GetObjectItemCaseSensitive(json, "price");

        if (!cJSON_IsString(name_item) || !cJSON_IsString(description_item) || !cJSON_IsNumber(price_item)) {
            send_response(client_socket, HTTP_BAD_REQUEST, "{\"error\": \"Missing or invalid fields\"}");
            cJSON_Delete(json);
            return;
        }

        const char *name = name_item->valuestring;
        const char *description = description_item->valuestring;
        float price = (float)price_item->valuedouble;

        // Update item
        int result = update_item(id, name, description, price);
        if (result == 0) {
            char response_body[1024];
            snprintf(response_body, sizeof(response_body), "{\"message\": \"Item updated\", \"id\": %d}", id);
            send_json_response(client_socket, HTTP_OK, response_body);
        } else {
            send_response(client_socket, HTTP_NOT_FOUND, "{\"error\": \"Item not found\"}");
        }

        cJSON_Delete(json);
    } else {
        send_response(client_socket, HTTP_BAD_REQUEST, "{\"error\": \"Invalid request\"}");
    }
}

void handle_patch_request(int client_socket, const char *path, const char *body) {
    int id;
    if (sscanf(path, "/items/%d", &id) == 1) {

        cJSON *json = cJSON_Parse(body);
        if (json == NULL) {
            send_response(client_socket, HTTP_BAD_REQUEST, "{\"error\": \"Invalid JSON format\"}");
            return;
        }

        const cJSON *name = cJSON_GetObjectItemCaseSensitive(json, "name");
        const cJSON *description = cJSON_GetObjectItemCaseSensitive(json, "description");
        const cJSON *price = cJSON_GetObjectItemCaseSensitive(json, "price");

        Item *item = get_item(id);
        if (item) {
            if (name && cJSON_IsString(name)) {
                strncpy(item->name, name->valuestring, sizeof(item->name));
            }
            if (description && cJSON_IsString(description)) {
                strncpy(item->description, description->valuestring, sizeof(item->description));
            }
            if (price && cJSON_IsNumber(price)) {
                item->price = price->valuedouble;
            }

            char response_body[1024];
            snprintf(response_body, sizeof(response_body),
                     "{\"message\": \"Item partially updated\", \"id\": %d, \"name\": \"%s\", \"description\": \"%s\", \"price\": %.2f}",
                     item->id, item->name, item->description, item->price);

            send_json_response(client_socket, HTTP_OK, response_body);
        } else {
            send_response(client_socket, HTTP_NOT_FOUND, "{\"error\": \"Item not found\"}");
        }

        cJSON_Delete(json);
    } else {
        send_response(client_socket, HTTP_BAD_REQUEST, "{\"error\": \"Invalid request\"}");
    }
}

void handle_delete_request(int client_socket, const char *path) {
    int id;
    if (sscanf(path, "/items/%d", &id) == 1) {
        int result = delete_item(id);
        if (result == 0) {
            send_response(client_socket, HTTP_OK, "{\"message\": \"Item deleted\"}");
        } else {
            send_response(client_socket, HTTP_NOT_FOUND, "{\"error\": \"Item not found\"}");
        }
    } else {
        send_response(client_socket, HTTP_BAD_REQUEST, "{\"error\": \"Invalid request\"}");
    }
}
