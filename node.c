#include <stdlib.h>

#include "node.h"

node_t* generate_simple_node(node_type_t type)
{
    node_t* node = calloc(1, sizeof(node_t));
    node->type = type;
    node->next = NULL;

    return node;
}
