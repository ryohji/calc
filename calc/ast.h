#pragma once

#include <stdio.h>

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif

typedef struct node node_t;

EXTERN_C node_t *node_new_number(double value);
EXTERN_C node_t *node_new_negation(node_t *node);
EXTERN_C node_t *node_new_addition(node_t *a, node_t *b);

EXTERN_C double node_value(node_t const *node);
EXTERN_C void node_fwrite(FILE *fp, node_t const *node);

EXTERN_C void node_addref(node_t *node);
EXTERN_C void node_release(node_t *node);
