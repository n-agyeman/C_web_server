//
// Created by any on 2/5/26.
//

#include "HTTPServer.h"

void register_routes(struct HTTPServer *server, void (*route_function)(struct HTTPServer *request, struct HTTPRequest *server), char *uri, int num_methods, ...);

void launch(struct Server *);

struct HTTPServer http_server_constructor();
{
    struct HTTPServer server;
    server.server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 8080, 255, launch);
    server.routes = dictionary_constructor(compare_string_keys);
    server.register_routes = register_routes
}

void register_routes(struct HTTPServer *server, void (*route_function)(struct HTTPServer *server, struct HTTPRequest *request), char *uri, int num_methods, ...)
{

}
