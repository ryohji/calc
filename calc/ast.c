#include "ast.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum node_type {
    NODE_NUMBER,
    NODE_NAGATION,
    NODE_ADDITION,
};

struct node_number {
    double const value;
};

struct node_pair {
    node_t *const a;
    node_t *const b;
};

struct node {
    enum node_type const type;
    unsigned reference;

    union {
        struct node_number const number;
        node_t *const node;
        struct node_pair const pair;
    } data;
};

static inline node_t *node_dup(node_t node) {
    node_t *p = malloc(sizeof(node));
    memcpy(p, &node, sizeof(node));
    return p;
}

node_t *node_new_number(double value) {
    return node_dup((node_t){.type = NODE_NUMBER, .reference = 1, .data.number = {value}});
}

node_t *node_new_negation(node_t *node) {
    node_addref(node);
    return node_dup((node_t){.type = NODE_NAGATION, .reference = 1, .data.node = node});
}

node_t *node_new_addition(node_t *a, node_t *b) {
    node_addref(a);
    node_addref(b);
    return node_dup((node_t){.type = NODE_ADDITION, .reference = 1, .data.pair = {a, b}});
}

double node_value(node_t const *node) {
    switch (node->type) {
    case NODE_NUMBER:
        return node->data.number.value;

    case NODE_NAGATION:
        return -node_value(node->data.node);

    case NODE_ADDITION:
        return node_value(node->data.pair.a) + node_value(node->data.pair.b);

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
        switch (node->type) {
        case NODE_NAGATION:
            node_release(node->data.node);
            break;

        case NODE_ADDITION:
            node_release(node->data.pair.a);
            node_release(node->data.pair.b);
            break;

        default:
            break;
        }
        free(node);
    }
}
