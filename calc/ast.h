#pragma once

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C extern
#endif

typedef struct node node_t;

EXTERN_C node_t *node_new_number(double value);

EXTERN_C double node_value(const node_t *node);

EXTERN_C void node_addref(node_t *node);
EXTERN_C void node_release(node_t *node);
