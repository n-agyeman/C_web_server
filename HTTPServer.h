//
// Created by any on 2/5/26.
//

#ifndef C_HTTPSERVER_H
#define C_HTTPSERVER_H

#include "Server.h"
#include "HTTPRequest.h"

struct HTTPServer
{
    struct Server server;
    struct Dictionary routes;

    void(*register_routes)(struct HTTPServer *server, void (*route_function)(struct HTTPServer *server, struct HTTPRequest *request), char *uri, int num_methods, ...);
};

struct HTTPServer http_server_constructor();


#endif //C_HTTPSERVER_H