#include <gumbo.h>

GumboNode **children(GumboNode *node, unsigned int *length);

GumboNode *nth_child(GumboNode *node, unsigned int n);

/* returns null if element has no id */
const char *elem_id(GumboNode *node);

/* Breadth-first search for any sub-element with given id */
/* Checks the given node as well */
GumboNode *elem_by_id(GumboNode *node, const char *id);

GumboNode **elems_by_attr(GumboNode *node,
        const char *attribute, const char *value, 
        unsigned int *num_elements);

GumboNode *first_elem_by_attr(GumboNode *node,
        const char *attribute, const char *value);

