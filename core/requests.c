#include <stdlib.h>
#include "requests.h"

/* Precondition: usrbuf points to a buffer. The write will start at
 * buffer->len*/
size_t _get_callback(void *buf, size_t size, size_t num, void *usrbuf) {
    struct Buffer *buffer = usrbuf; 
    Buffer_append(buffer, buf, size * num);
    return size * num;
}

int Requests_init() {
    return curl_global_init(CURL_GLOBAL_ALL);
}

struct Requests *Requests_new() {
    /* Initialize all curl objects */
    struct Requests *request = malloc(sizeof(struct Requests));
    int i;
    for (i = 0; i < METHOD_COUNT; i++)
        request->curl[i] = curl_easy_init();
        
    /* Set up GET method*/
    curl_easy_setopt(request->curl[GET], CURLOPT_WRITEFUNCTION, _get_callback);
    curl_easy_setopt(request->curl[GET], CURLOPT_FOLLOWLOCATION, 1);

    return request;
}

void Requests_destroy(struct Requests *request) {
    /* Clean up all Curl objects */
    int i;
    for (i = 0; i < METHOD_COUNT; i++)
        curl_easy_cleanup(request->curl[i]);
    free(request);
}

/* Simple get function, nothing fancy */
int HTTP_get(struct Requests *request, char *url, struct Buffer *buffer) {
    curl_easy_setopt(request->curl[GET], CURLOPT_URL, url);
    curl_easy_setopt(request->curl[GET], CURLOPT_WRITEDATA, buffer);
    CURLcode status = curl_easy_perform(request->curl[GET]);

    /* Curl kindly does not null terminate your strings. We decided to be a
     * nuisance and do that for you */
    Buffer_append(buffer, "", 1);
}

/*
 * POST Methods
 * TODO: add support for custom HTTP Headers
 */


/* Simple function to post strings */
int HTTP_post_str(struct Requests *request, char *url, char *post_data) {
    curl_easy_setopt(request->curl[POST], CURLOPT_URL, url);
    curl_easy_setopt(request->curl[POST], CURLOPT_POSTFIELDS, post_data);
    return curl_easy_perform(request->curl[POST]);
}

/* Simple funciton to post binary data */
int HTTP_post_bin(struct Requests *request, char *url, void *post_data, size_t post_len) {
    curl_easy_setopt(request->curl[POST], CURLOPT_URL, url);
    curl_easy_setopt(request->curl[POST], CURLOPT_POSTFIELDS, post_data);
    curl_easy_setopt(request->curl[POST], CURLOPT_POSTFIELDSIZE, post_len);
    return curl_easy_perform(request->curl[POST]);
}
