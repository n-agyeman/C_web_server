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
#include <stdio.h>


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
    //printf("Entered constructor\n");
    struct HTTPServer server;
    //printf("Instantiated server\n");
    server.server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 8080, 255);
    //printf("used server constructor\n");
    server.routes = dictionary_constructor(compare_string_keys);
    //printf("attached server routes\n");
    server.register_routes = register_routes;
    //printf("registered server routes\n");
    server.launch = launch;
    //printf("launched server\n");
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
    va_end(methods);
    // allocate and copy uri
    route.uri = strdup(uri);
    //strcpy(route.uri, uri);
    route.route_function = route_function;
    server->routes.insert(&server->routes, uri, sizeof(char[strlen(uri)]), &route, sizeof(route));
}

void launch(struct HTTPServer *server)
{
    struct ThreadPool* thread_pool = thread_pool_constructor(8);
    struct sockaddr *sock_addr = (struct sockaddr *)&server->server.address;
    socklen_t address_length = (socklen_t)sizeof(server->server.address);
    //printf("launch. entering while loop\n");
    while (1)
    {
        printf("launch.while entered while loop\n");
        struct ClientServer *client_server = malloc(sizeof(struct ClientServer));
        printf("launch.while created a clientServer\n");
        client_server->client = accept(server->server.socket, sock_addr, &address_length);
        printf("launch.while accepted request\n");
        client_server->server = server;
        printf("launch.while assigned server\n");
        struct ThreadJob job = thread_job_constructor(handler, client_server);
        printf("launch.while joined thread\n");
        thread_pool->add_work(thread_pool, job);
    }
    printf("launch. left while loop");
}

void * handler(void *arg)
{
    printf("handler thread\n");
    struct ClientServer *client_server = (struct ClientServer *)arg;
    char request_string[500];
    int bytes = read(client_server->client, request_string, 500-1);
    if (bytes <= 0)
    {
        close(client_server->client);
        free(client_server);
        return NULL;
    }
    request_string[bytes] = '\0';
    printf("%s\n",request_string);
    struct HTTPRequest request = http_request_constructor(request_string);
    char *uri = request.request_line.search(&request.request_line, "uri", sizeof("uri"));
    struct Route *route = client_server->server->routes.search(&client_server->server->routes, uri, sizeof(char[strlen(uri)]));
    char *response = route->route_function(client_server->server, &request);

    //http headers
    unsigned long len = strlen(response);

    char header[512];
    sprintf(header,
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: %lu\r\n"
        "Connection: close\r\n"
        "\r\n",
        len);
    write(client_server->client, header, strlen(header));
    write(client_server->client, response, len);
    free(client_server);
    return NULL;
}

// Joins the contents of multiple files into one.
char *render_template(int num_templates, ...)
{
    // Create a buffer to store the data in.
    char *buffer = malloc(30000);
    int buffer_position = 0;
    char c;
    FILE *file;
    // Iterate over the files given as arguments.
    va_list files;
    va_start(files, num_templates);
    // Read the contents of each file into the buffer.
    for (int i = 0; i < num_templates; i++)
    {
        char *path = va_arg(files, char*);
        file = fopen(path, "r");
        while ((c = fgetc(file)) != EOF)
        {
            buffer[buffer_position] = c;
            buffer_position += 1;
        }
    }
    va_end(files);
    return buffer;
}
