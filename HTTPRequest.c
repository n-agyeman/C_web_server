//
// Created by any on 2/4/26.
//

#include "HTTPRequest.h"

#include <stdlib.h>
#include <string.h>

#include "DataStructures/Lists/Queue.h"

#include "Server.h"

// take a method and return its corresponding methon in the http method enum
int method_select(char *method)
{
    if (strcmp(method, "GET") == 0)
        return GET;
    else if (strcmp(method, "POST") == 0)
        return POST;
    else if (strcmp(method, "PUT") == 0)
        return PUT;
    else if (strcmp(method, "HEAD") == 0)
        return HEAD;
    else if (strcmp(method, "PATCH") == 0)
        return PATCH;
    else if (strcmp(method, "DELETE") == 0)
        return DELETE;
    else if (strcmp(method, "OPTIONS") == 0)
        return OPTIONS;
    else if (strcmp(method, "TRACE") == 0)
        return TRACE;
    else return -1;
}

struct HTTPRequest http_request_constructor(char *request_string)
{
    struct HTTPRequest request;

    char requested[strlen(request_string)];
    strcpy(requested, request_string); // create a variable to store a copy of request_string since it's stored as a string literal

    // replace the line between the header_fields and body with '|' for tokenizatioins
    for (int i = 0; i <= strlen(requested) - 1; i++)
    {
        if (requested[i] == '\n' && requested[i +1] == '\n')
            requested[i+1] = '|';
    }

    //separate different sections of the http string into tokens
    char *request_line = strtok(requested, " \n");
    char *header_fields = strtok(NULL, "|");
    char *body = strtok(NULL, "|");

    extract_request_line_fields(&request, requested);
    extract_header_fields(&request, header_fields);
    extract_body(&request, body);

    // //create dictionary to store header fields as key value pairs
    // request.header_fields = dictionary_constructor(compare_string_keys);
    //
    // // loop through each line in header_fields and add them to a queue
    // struct Queue headers = queue_constructor();
    //
    // char *token = strtok(request_line, "\n");
    // while (token)
    // {
    //     headers.push(&headers, token, sizeof(token));
    //     token = strtok(NULL, "\n");
    // }
    //
    // // get each item in header then split them into key value pair, add them to header_fields dictionary
    // char *header = (char *)headers.peek(&headers);
    // while (header)
    // {
    //     char *key = strtok(header, ":");
    //     char *value = strtok(NULL, "|");
    //     headers.pop(&headers);
    //     request.header_fields.insert(&request.header_fields, key, sizeof(*key), value, sizeof(*value));
    // }


    return request;
}

// Parses out the request line to retrieve the method, uri, and http version.
void extract_request_line_fields(struct HTTPRequest *request, char *request_line)
{
    // Copy the string literal into a local instance.
    char fields[strlen(request_line)];
    strcpy(fields, request_line);
    // Separate the string on spaces for each section.
    char *method = strtok(fields, " ");
    char *uri = strtok(NULL, " ");
    char *http_version = strtok(NULL, "\0");
    // Insert the results into the request object as a dictionary.
    struct Dictionary request_line_dict = dictionary_constructor(compare_string_keys);
    request_line_dict.insert(&request_line_dict, "method", sizeof("method"), method, sizeof(char[strlen(method)]));
    request_line_dict.insert(&request_line_dict, "uri", sizeof("uri"), uri, sizeof(char[strlen(uri)]));
    request_line_dict.insert(&request_line_dict, "http_version", sizeof("http_version"), http_version, sizeof(char[strlen(http_version)]));
    // Save the dictionary to the request object.
    request->request_line = request_line_dict;
    if (request->request_line.search(&request->request_line, "GET", sizeof("GET")))
    {
        extract_body(request, (char *)request->request_line.search(&request->request_line, "uri", sizeof("uri")));
    }
}

// Parses out the header fields.
void extract_header_fields(struct HTTPRequest *request, char *header_fields)
{
    // Copy the string literal into a local instance.
    char fields[strlen(header_fields)];
    strcpy(fields, header_fields);
    // Save each line of the input into a queue.
    struct Queue headers = queue_constructor();
    char *field = strtok(fields, "\n");
    while (field)
    {
        headers.push(&headers, field, sizeof(char[strlen(field)]));
        field = strtok(NULL, "\n");
    }
    // Initialize the request's header_fields dictionary.
    request->header_fields = dictionary_constructor(compare_string_keys);
    // Use the queue to further extract key value pairs.
    char *header = (char *)headers.peek(&headers);
    while (header)
    {
        char *key = strtok(header, ":");
        char *value = strtok(NULL, "\0");
        if (value)
        {
            // Remove leading white spaces.
            if (value[0] == ' ')
            {
                value++;
            }
            // Push the key value pairs into the request's header_fields dictionary.
            request->header_fields.insert(&request->header_fields, key, sizeof(char[strlen(key)]), value, sizeof(char[strlen(value)]));
            // Collect the next field from the queue.
        }
        headers.pop(&headers);
        header = (char *)headers.peek(&headers);
    }
    // Destroy the queue.
    queue_destructor(&headers);
}

// Parses the body according to the content type specified in the header fields.
void extract_body(struct HTTPRequest *request, char *body)
{
    // Check what content type needs to be parsed
    char *content_type = (char *)request->header_fields.search(&request->header_fields, "Content-Type", sizeof("Content-Type"));
    if (content_type)
    {
        // Initialize the body_fields dictionary.
        struct Dictionary body_fields = dictionary_constructor(compare_string_keys);
        if (strcmp(content_type, "application/x-www-form-urlencoded") == 0)
        {
            // Collect each key value pair as a set and store them in a queue.
            struct Queue fields = queue_constructor();
            char *field = strtok(body, "&");
            while (field)
            {
                fields.push(&fields, field, sizeof(char[strlen(field)]));
            }
            // Iterate over the queue to further separate keys from values.
            field = fields.peek(&fields);
            while (field)
            {
                char *key = strtok(field, "=");
                char *value = strtok(NULL, "\0");
                // Remove unnecessary leading white space.
                if (value[0] == ' ')
                {
                    value++;
                }
                // Insert the key value pair into the dictionary.
                body_fields.insert(&body_fields, key, sizeof(char[strlen(key)]), value, sizeof(char[strlen(value)]));
                // Collect the next item in the queue.
                fields.pop(&fields);
                field = fields.peek(&fields);
            }
            // Destroy the queue.
            queue_destructor(&fields);
        }
        else
        {
            // Save the data as a single key value pair.
            body_fields.insert(&body_fields, "data", sizeof("data"), body, sizeof(char[strlen(body)]));
        }
        // Set the request's body dictionary.
        request->body = body_fields;
    }
}

// split request_line to method, and (uri + http version)
// find_method and store as method
// extract and assign uri from the remaining request line
// extract httpversion from remaining request line and store as float
