// //
// // Created by any on 2/3/26.
// //
//
// #include <stdio.h>
// #include <string.h>
// #include <unistd.h>
//
// #include "Server.h"
//
// void launch(struct Server *server)
// {
//     printf("Launching server\n");
//     int address_length = sizeof(server->address);
//     int new_socket;
//
//     char* hello = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: 61\r\nConnection: close\r\n\r\n<h1>Hello World!, I just built a webserver with C programming! </h1>";
//     char buffer[100];
//     while (1)
//     {
//         printf("===== WAITING FOR CONNECTION =====\n");
//         new_socket = accept(server->socket, (struct sockaddr*)&server->address, (socklen_t*)&address_length);
//         if (new_socket == -1)
//             error("Failed to accept new socket");
//
//         read(new_socket, buffer, sizeof(buffer)-1);
//         buffer[strlen(buffer)] = '\0';
//         printf("%s\n", buffer);
//
//         write(new_socket, hello, strlen(hello));
//         close(new_socket);
//     }
// }
//
// int main() {
//     struct Server server = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 8080, 10, launch);
//     server.launch(&server);
// }