#include <stdio.h>

#include "DL_list_err_proc.h"
#include "DL_list_proc.h"
#include "general.h"

void DL_list_add_err(enum DL_list_err_t *dest, enum DL_list_err_t add) {
    *dest = (DL_list_err_t)((unsigned long long)(*dest) | (unsigned long long) add);
}

void DL_list_err_get_descr(const enum DL_list_err_t err_code, char err_descr_str[]) {
    bool error = false;
    #define DESCR_(err_code, err)                 \
        {                                         \
            if (err_code & err) {                 \
                sprintf(err_descr_str, #err", "); \
                error = true;                     \
            }                                     \
        }                                         \

    DESCR_(err_code, DL_ERR_ALLOC);
    DESCR_(err_code, DL_ERR_FILE_OPEN);
    DESCR_(err_code, DL_ERR_PUSH);
    DESCR_(err_code, DL_ERR_INSERT);
    DESCR_(err_code, DL_ERR_CYCLED);
    DESCR_(err_code, DL_ERR_INVALID_NODE);
    DESCR_(err_code, DL_ERR_INVALID_NODE_CONNECTION);
    DESCR_(err_code, DL_ERR_STACK);
    DESCR_(err_code, DL_ERR_SYSTEM);
    DESCR_(err_code, DL_ERR_REALLOC);

    if (!error) {
        sprintf(err_descr_str, "ALL IS OK:)");
    }
    #undef DESCR_
}

DL_list_err_t DL_list_verify(const DL_list_t *list) {
    DL_list_err_t errors = DL_ERR_OK;

    for (size_t i = 0; i < list->size; i++) {
        DL_list_node_t *node = &list->data[i];
        if (node->empty) {
            continue;
        }
        if (node->prev->next != node) {
            DL_list_add_err(&errors, DL_ERR_INVALID_NODE_CONNECTION);
            DEBUG_DL_LIST_ERROR(DL_ERR_INVALID_NODE, "nodes '%p' and '%p' connection invalid", node->prev, &node);
        }
        if (node->next->prev != node) {
            DL_list_add_err(&errors, DL_ERR_INVALID_NODE_CONNECTION);
            DEBUG_DL_LIST_ERROR(DL_ERR_INVALID_NODE, "nodes '%p' and '%p' connection invalid", node->next, &node);
        }
    }

    DL_list_node_t *node = &list->data[0];
    node = node->next;

    size_t iterations = 0;

    while (node != &list->data[0]) {
        if (iterations >= MAX_CYCLE_ITERATIONS) {
            DL_list_add_err(&errors, DL_ERR_CYCLED);
            DEBUG_DL_LIST_ERROR(DL_ERR_CYCLED, "")
            break;
        }
        if (node->next == NULL) {
            DL_list_add_err(&errors, DL_ERR_INVALID_NODE);
            DEBUG_DL_LIST_ERROR(DL_ERR_INVALID_NODE,
                "reachable node has invalid parameters: addr: {%p}, next: {%p} prev: {%p}", node, node->next, node->prev)
            break;
        }
        node = node->next;
        iterations++;
    }

    return errors;
}
