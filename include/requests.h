#ifndef REQUESTS_H
#define REQUESTS_H

#include <curl/curl.h>
#include "buffer.h"

enum Request_method {
    GET,
    POST,
    PUT,
    DELETE,
    PATCH,
    METHOD_COUNT /* Must always be last */
};

struct Requests {
    CURL *curl[METHOD_COUNT];
};

int Requests_init();

struct Requests *Requests_new();

void Requests_destroy(struct Requests *request);

/* Simple get function, nothing fancy */
int HTTP_get(struct Requests *request, char *url, struct Buffer *buffer);

/*
 * POST Methods
 * TODO: add support for custom HTTP Headers
 */

/* Simple function to post strings */
int HTTP_post_str(struct Requests *request, char *url, char *post_data);

/* Simple funciton to post binary data */
int HTTP_post_bin(struct Requests *request, char *url, void *post_data, size_t post_len);

#endif
