//
// Created by any on 2/5/26.
//

#include "HTTPServer.h"
#include "Route.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <Systems.h>
#include <unistd.h>
#include <sys/socket.h>

void * handler(void *arg);
void launch(struct HTTPServer *server);

void register_routes(struct HTTPServer *server, char * (*route_function)(struct HTTPServer *request, struct HTTPRequest *server), char *uri, int num_methods, ...);

//void launch(struct Server *);
struct ClientServer
{
    int client;
    struct HTTPServer *server;
};


struct HTTPServer http_server_constructor()
{
    struct HTTPServer server;
    server.server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 8080, 255);
    server.routes = dictionary_constructor(compare_string_keys);
    server.register_routes = register_routes;
    server.launch = launch;
    return server;
}

void register_routes(struct HTTPServer *server, char * (*route_function)(struct HTTPServer *server, struct HTTPRequest *request), char *uri, int num_methods, ...)
{
    struct Route route;
    va_list methods;
    va_start(methods, num_methods);
    for (int i = 0; i < num_methods; i++)
    {
        route.methods[i] = va_arg(methods, int);
    }
    strcpy(route.uri, uri);
    route.route_function = route_function;
    server->routes.insert(&server->routes, uri, sizeof(char[strlen(uri)]), &route, sizeof(route));
}

void launch(struct HTTPServer *server)
{
    struct ThreadPool thread_pool = thread_pool_constructor(8);
    struct sockaddr *sock_addr = (struct sockaddr *)&server->server.address;
    socklen_t address_length = (socklen_t)sizeof(server->server.address);
    while (1)
    {
        struct ClientServer *client_server = malloc(sizeof(struct ClientServer));
        client_server->client = accept(server->server.socket, sock_addr, &address_length);
        client_server->server = server;
        struct ThreadJob job = thread_job_constructor(handler, client_server);
        thread_pool.add_work(&thread_pool, job);
    }
}

void * handler(void *arg)
{
    struct ClientServer *client_server = (struct ClientServer *)arg;
    char request_string[30000];
    read(client_server->client, request_string, 30000);
    struct HTTPRequest request = http_request_constructor(request_string);
    char *uri = request.request_line.search(&request.request_line, "uri", sizeof("uri"));
    struct Route *route = client_server->server->routes.search(&client_server->server->routes, uri, sizeof(char[strlen(uri)]));
    char *response = route->route_function(client_server->server, &request);
    write(client_server->client, response, sizeof(char[strlen(response)]));
    free(client_server);
    return NULL;
}
