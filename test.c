#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gumbo.h>
#include "requests.h"
#include "api.h"

/* Performs a GET on a URL */
int main(int argc, char **argv) {
    /* Args processing*/
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <subreddit>\n", argv[0]);
        return 1;
    }
    char *subreddit = argv[1];
    int url_len = strlen(subreddit) + 20;
    char *url = malloc(url_len);
    snprintf(url, url_len, "www.reddit.com/r/%s", subreddit); 

    /* Initialize Requests and Data Structures */
    Requests_init();

    struct Buffer *b = Buffer_new(0);
    struct Requests *r = Requests_new();
    
    /* Simple get request */
    HTTP_get(r, url, b);

    /* Parse HTML */
    GumboOutput *dom = gumbo_parse(b->data);

    /* Grab top post in selected subreddit */
    const char *top_post_in_subreddit = top_post(dom, NULL);
    printf("%s\n", top_post_in_subreddit);

    /* Cleanup */
    gumbo_destroy_output(&kGumboDefaultOptions, dom);
    free(url);
    Buffer_destroy(b);
    Requests_destroy(r);
}
