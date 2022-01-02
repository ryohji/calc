#include "ast.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum node_type {
    NODE_NUMBER,
    NODE_NAGATE,
};

struct node_number {
    double const value;
};

struct node {
    enum node_type const type;
    unsigned reference;

    union {
        struct node_number const number;
        node_t *const node;
    } value;
};

static inline node_t *node_dup(node_t node) {
    node_t *p = malloc(sizeof(node));
    memcpy(p, &node, sizeof(node));
    return p;
}

node_t *node_new_number(double value) {
    return node_dup((node_t){.type = NODE_NUMBER, .reference = 1, .value.number = {value}});
}

node_t *node_new_negation(node_t *node) {
    node_addref(node);
    return node_dup((node_t){.type = NODE_NAGATE, .reference = 1, .value.node = node});
}

double node_value(node_t const *node) {
    switch (node->type) {
    case NODE_NUMBER:
        return node->value.number.value;

    case NODE_NAGATE:
        return -node_value(node->value.node);

    default: {
        char message[1024];
        strerror_r(EINVAL, message, sizeof(message));
        fputs(message, stderr);
        exit(1);
    } break;
    }
}

void node_addref(node_t *node) {
    node->reference += 1;
}

void node_release(node_t *node) {
    if (--node->reference == 0) {
        if (node->type == NODE_NAGATE) {
            node_release(node->value.node);
        }
        free(node);
    }
}
