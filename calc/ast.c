#include "ast.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum node_type {
    NODE_NUMBER,
};

struct node_number {
    double const value;
};

struct node {
    enum node_type const type;
    unsigned reference;

    union {
        struct node_number const number;
    } value;
};

node_t *node_new_number(double value) {
    node_t n = {.type = NODE_NUMBER, .reference = 1, .value.number = {value}};
    node_t *p = malloc(sizeof(node_t));
    memcpy(p, &n, sizeof(n));
    return p;
}

double node_value(const node_t *node) {
    switch (node->type) {
    case NODE_NUMBER:
        return node->value.number.value;

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
        free(node);
    }
}
