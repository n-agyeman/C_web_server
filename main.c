//
// Created by any on 2/5/26.
//

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
    struct HTTPServer server = http_server_constructor();

    server.register_routes(&server, home, "/", 0);
    server.register_routes(&server, about, "/", 0);

    server.launch(&server);

}