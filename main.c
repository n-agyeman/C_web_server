//
// Created by any on 2/5/26.
//

#include <stdio.h>

#include "HTTPServer.h"

char * home(struct HTTPServer *server, struct HTTPRequest *request)
{
    char *x = render_template(1, "/home/any/code/javascript/jcc/index.html");
   // char *x = render_template(3, "/home/any/code/javascript/jcc/index.html", "/home/any/code/javascript/style.css", "/home/any/code/javascript/jcc/script.js");
    return x;
}

char * refresh(struct HTTPServer *server, struct HTTPRequest *request)
{
    char *x = render_template(1, "/home/any/code/javascript/jcc/index.html");
    // char *x = render_template(3, "/home/any/code/javascript/jcc/index.html", "/home/any/code/javascript/style.css", "/home/any/code/javascript/jcc/script.js");
    return x;
}

char * about(struct HTTPServer *server, struct HTTPRequest *request)
{
    return render_template(2, "/about", "about");
}

int main()
{
    //printf("Entered main\n");
    struct HTTPServer server = http_server_constructor();
    //printf("created server constructor");

    server.register_routes(&server, home, "/home", 0);
    server.register_routes(&server, refresh, "/favicon.ico", 0);
    //printf("registered first route");

    server.register_routes(&server, about, "/about", 0);
    //printf("registered second route");

    server.launch(&server);
    //printf("launched server");

}