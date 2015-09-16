#ifndef BUFFER_H
#define Buffer_H

#include <stdint.h>

#define DEFAULT_BUFFER_SIZE 4096

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

void Buffer_pop(struct Buffer *b, void *ptr, uint64_t len);

/* Completely overwrites b with len bytes from ptr */
void Buffer_copy(struct Buffer *b, void *ptr, uint64_t len);

/* Resizes Buffer until its capacity equals its length */
void Buffer_trim(struct Buffer *b);

#endif
