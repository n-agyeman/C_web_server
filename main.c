//
// Created by any on 2/5/26.
//

#include <stdio.h>

#include "HTTPServer.h"

char * home(struct HTTPServer *server, struct HTTPRequest *request)
{
    char *x = render_template(2, "/Us", "user");
    return x;
}

char * about(struct HTTPServer *server, struct HTTPRequest *request)
{
    return render_template(2, "/about", "about");
}

int main()
{
    printf("Entered main\n");
    struct HTTPServer server = http_server_constructor();
    printf("created server constructor");

    server.register_routes(&server, home, "/", 0);
    printf("registered first route");

    server.register_routes(&server, about, "/", 0);
    printf("registered second route");

    server.launch(&server);
    printf("launched server");

}