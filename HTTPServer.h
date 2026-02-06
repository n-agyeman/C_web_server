//
// Created by any on 2/5/26.
//

#ifndef C_HTTPSERVER_H
#define C_HTTPSERVER_H

#include "Server.h"
#include "HTTPRequest.h"
#include "../DataStructures.h"
#include "../Systems.h"
#include "../DataStructures/Dictionary/Dictionary.h"


struct HTTPServer
{
    struct Server server;
    struct Dictionary routes;

    void(*register_routes)(struct HTTPServer *server, char * (*route_function)(struct HTTPServer *server, struct HTTPRequest *request), char *uri, int num_methods, ...);

    void (*launch)(struct HTTPServer *server);

};

struct HTTPServer http_server_constructor();
char *render_template(int num_templates, ...);
#endif //C_HTTPSERVER_H