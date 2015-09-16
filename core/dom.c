#include <string.h>
#include <stdlib.h>
#include "dom.h"
#include "buffer.h"

/* Returns the array of children, puts the length in length */
/* May want to extend to document, since those have children too */
GumboNode **children(GumboNode *node, unsigned int *length) {
    if (node->type != GUMBO_NODE_ELEMENT)
        return NULL;
    *length = node->v.document.children.length;
    return (GumboNode **)node->v.element.children.data;
}

/* Get the nth immediate child element */
GumboNode *nth_child(GumboNode *node, unsigned int n) {
    unsigned int length;
    GumboNode **node_children = children(node, &length);
    if (node_children == NULL || n >= length)
        return NULL;
    return node_children[n];
}

/* returns null if element has no id */
const char *elem_id(GumboNode *node) {
    if (node->type != GUMBO_NODE_ELEMENT)
        return NULL;
    GumboAttribute **attributes = 
        (GumboAttribute **)node->v.element.attributes.data;
    unsigned int i, length = node->v.element.attributes.length;
    for (i = 0; i < length; i++) {
        GumboAttribute *attribute = attributes[i];
        if (strncmp(attribute->name, "id", 3))
            continue;
        return attribute->value;
    }
    return NULL;
}

const char *elem_attr_value(GumboNode *node, const char *attribute_name) {
    if (node->type != GUMBO_NODE_ELEMENT)
        return NULL;
    GumboAttribute **attributes = 
        (GumboAttribute **)node->v.element.attributes.data;
    unsigned int i, length = node->v.element.attributes.length,
                 attr_length = strlen(attribute_name) + 1;
    for (i = 0; i < length; i++) {
        GumboAttribute *attribute = attributes[i];
        if (strncmp(attribute->name, attribute_name, attr_length))
            continue;
        return attribute->value;
    }
    return NULL;
}

/* Breadth-first search for any sub-element with given id */
/* Checks the given node as well */
GumboNode *elem_by_id(GumboNode *node, const char *id) {
    unsigned int id_length = strlen(id) + 1;
    struct Buffer *queue = Buffer_new(0);
    /* Push node */
    Buffer_append(queue, &node, sizeof(node));
    GumboNode *current;
    unsigned int i, length;
    const char *check_id;
    GumboNode *result = NULL;
    while(queue->len) {
        Buffer_pop(queue, &current, sizeof(current));
        /* Check id */ 
        check_id = elem_id(current);
        if (check_id && strncmp(id, check_id, id_length) == 0) {
            result = current;
            break;
        }
        GumboNode **node_children = children(current, &length);
        if (node_children == NULL)
            continue;
        for (i = 0; i < length; i++) {
            Buffer_append(queue, &node_children[i], sizeof(GumboNode *));
        }
    }
    Buffer_destroy(queue);
    return result;
}

GumboNode **elems_by_attr(GumboNode *node,
        const char *attribute, const char *value, 
        unsigned int *num_elements) {

    unsigned int value_length = strlen(value) + 1;
    struct Buffer *queue = Buffer_new(0), *elems_found = Buffer_new(0);
    /* Push node */
    Buffer_append(queue, &node, sizeof(node));
    GumboNode *current;
    unsigned int i, length;
    const char *check_value;
    while(queue->len) {
        Buffer_pop(queue, &current, sizeof(current));
        check_value = elem_attr_value(current, attribute);
        if (check_value && strncmp(value, check_value, value_length) == 0) {
            /* Add to list */
            Buffer_append(elems_found, &current, sizeof(current));
        }
        GumboNode **node_children = children(current, &length);
        if (node_children == NULL)
            continue;
        for (i = 0; i < length; i++) {
            Buffer_append(queue, &node_children[i], sizeof(GumboNode *));
        }
    }
    Buffer_trim(elems_found);
    GumboNode **result = elems_found->data;
    *num_elements = elems_found->len / sizeof(GumboNode *);
    free(elems_found);
    Buffer_destroy(queue);
    return result;
}

/* returns first elem with the attribute and value specified */
/* Should only be used if you know there is only one sub-element
 * with the specified attribute value */
GumboNode *first_elem_by_attr(GumboNode *node,
        const char *attribute, const char *value) {

    unsigned int value_length = strlen(value) + 1;
    struct Buffer *queue = Buffer_new(0);
    /* Push node */
    Buffer_append(queue, &node, sizeof(node));
    GumboNode *current;
    unsigned int i, length;
    const char *check_value;
    GumboNode *result = NULL;
    while(queue->len) {
        Buffer_pop(queue, &current, sizeof(current));
        check_value = elem_attr_value(current, attribute);
        if (check_value && strncmp(value, check_value, value_length) == 0) {
            result = current;
            break;
        }
        GumboNode **node_children = children(current, &length);
        if (node_children == NULL)
            continue;
        for (i = 0; i < length; i++) {
            Buffer_append(queue, &node_children[i], sizeof(GumboNode *));
        }
    }
    Buffer_destroy(queue);
    return result;
}
