#include "stdio.h"
#include "requests.h"

#define DEFAULT_BUF_SIZE 4096

/* Performs a GET on a URL */
int main(int argc, char **argv) {
    /* Args processing*/
    if (argc < 2) {
        fprintf(stderr, "Usage: ./%s <URL>\n", argv[0]);
        return 1;
    }
    char *url = argv[1];

    /* Initialize Requests and Data Structures */
    Requests_init();

    struct Buffer *b = Buffer_new(10 * DEFAULT_BUF_SIZE);
    struct Requests *r = Requests_new();
    
    /* Simple get request */
    HTTP_get(r, url, b);

    printf("%s\n", b->data);

    /* Cleanup */
    Buffer_destroy(b);
    Requests_destroy(r);
}
