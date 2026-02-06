//
// Created by any on 2/4/26.
//

#ifndef C_HTTPREQUEST_H
#define C_HTTPREQUEST_H

#include "../Systems.h"
#include "../DataStructures/Dictionary/Dictionary.h"
#include "DataStructures/Dictionary/Dictionary.h"
#include "../DataStructures.h"
#include "DataStructures/Dictionary/Dictionary.h"

enum HTTPMethod
{
    GET,
    POST,
    PUT,
    HEAD,
    PATCH,
    DELETE,
    OPTIONS,
    TRACE,
};

struct HTTPRequest
{
    struct Dictionary request_line;
    struct Dictionary header_fields;
    struct Dictionary body;
};


struct HTTPRequest http_request_constructor(char *request_string);

void extract_request_line_fields(struct HTTPRequest *request, char *request_line);
void extract_body(struct HTTPRequest *request, char *body);
void extract_header_fields(struct HTTPRequest *request, char *header_fields);

void http_request_destructor(struct HTTPRequest *request);


#endif //C_HTTPREQUEST_H

// check method