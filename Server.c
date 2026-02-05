//
// Created by any on 2/3/26.
//

#include "Server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>


void error(char *msg)
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(1);
}

struct Server server_constructor(int domain, int service, int protocol, u_long interface, int port, int backlog, void (*launch)(struct Server *server))
{
    struct Server server;
    server.domain = domain;
    server.service = service;
    server.protocol = protocol;
    server.interface = interface;
    server.port = port;
    server.backlog = backlog;

    server.address.sin_family = domain;
    server.address.sin_port = htons(port);
    server.address.sin_addr.s_addr = htonl(interface);

    server.socket = socket(server.domain, server.service, server.protocol);
    if (server.socket == -1)
    {
        error("Failed to create socket...");
    }

    // int reuse =1;
    // if (setsockopt(server.socket, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)))
    //     error("Can't set the reuse option on the socket");

    if ((bind(server.socket, (struct sockaddr *)&server.address, sizeof(server.address))) == -1)
        error("Failed to bind socket...");

    if ((listen(server.socket, server.backlog)) == -1)
        error("Failed to listen socket...");

    server.launch = launch;

    return server;
}