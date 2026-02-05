//
// Created by any on 2/5/26.
//

#ifndef C_ROUTES_H
#define C_ROUTES_H

#include "HTTPServer.h"
#include "HTTPRequest.h"

struct Route
{
    int methods[9];
    char *uri;
    char * (*route_function)(struct HTTPServer *server, struct HTTPRequest *request);
};

#endif //C_ROUTES_H