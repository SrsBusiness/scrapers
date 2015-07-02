#include <stdint.h>

struct Buffer {
    uint64_t len, cap;
    void *data;
};

/* "Constructor" function for  buffers */
struct Buffer *Buffer_new(uint64_t cap);

/* "Destructor" function for buffers */
void Buffer_destroy(struct Buffer *b);

/* Appends len bytes from ptr to b */
void Buffer_append(struct Buffer *b, void *ptr, uint64_t len);

/* Completely overwrites b with len bytes from ptr */
void Buffer_copy(struct Buffer *b, void *ptr, uint64_t len);
