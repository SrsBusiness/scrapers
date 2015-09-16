#include <stdlib.h>
#include <string.h>
#include "buffer.h"

static void _Buffer_resize(struct Buffer *b, uint64_t len);

struct Buffer *Buffer_new(uint64_t cap) {
    cap = cap ? cap : DEFAULT_BUFFER_SIZE;
    struct Buffer *b = malloc(sizeof(struct Buffer));
    b->len = 0;
    b->cap = cap;
    b->data = malloc(cap);
    return b;
}

void Buffer_destroy(struct Buffer *b) {
    free(b->data);
    free(b);
}

void Buffer_append(struct Buffer *b, void *ptr, uint64_t len) {
    if (len + b->len > b->cap) {
        _Buffer_resize(b, len + b->len);
    }
    memcpy((void *)((char *)b->data + b->len), ptr, len);
    b->len += len;
}

void Buffer_pop(struct Buffer *b, void *ptr, uint64_t len) {
    if (len > b->len)
        return;
    memcpy(ptr, (void *)((char *)b->data + b->len - len), len);
    b->len -= len;
}

void Buffer_copy(struct Buffer *b, void *ptr, uint64_t len) {
    if (len > b->cap) {
        _Buffer_resize(b, len);
    }
    memcpy(b->data, ptr, len);
    b->len = len;
}

/* Resizes buffer such that cap == len */
void Buffer_trim(struct Buffer *b) {
    void *new_arr = malloc(b->len);
    memcpy(new_arr, b->data, b->len);
    free(b->data);
    b->data = new_arr;
    b->cap = b->len;
}

/* doubles capacity until it is greater than len */
static void _Buffer_resize(struct Buffer *b, uint64_t len) {
    while (len > b->cap) {
        b->cap *= 2;
    }
    /* No realloc since we need to copy the data from the old pointer */
    void *new_arr = malloc(b->cap);
    memcpy(new_arr, b->data, b->len);
    free(b->data);
    b->data = new_arr;
}
