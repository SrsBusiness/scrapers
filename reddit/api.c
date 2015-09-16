#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "api.h"
#include "dom.h"

const char *top_post(GumboOutput *dom, struct Buffer *title) {
    assert(dom->root->type == GUMBO_NODE_ELEMENT);
    GumboNode *table = elem_by_id(dom->root, "siteTable");
    unsigned int i, length, tmp;
    GumboNode **node_children = children(table, &length);
    for (i = 0; i < length; i++) {
        /* Check if rank 1 */
        GumboNode *span = first_elem_by_attr(node_children[i],
                "class", "rank");
        if (!span)
            continue;
        GumboNode *html = nth_child(span, 0);
        if (!html)
            continue;
        if (strncmp("1", html->v.text.text, 2) != 0)
            continue;
        GumboNode *entry = first_elem_by_attr(node_children[i],
                "class", "title may-blank ");
        html = nth_child(entry, 0);
        return html->v.text.text;
    }
}
