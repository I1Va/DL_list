#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdlib.h>

#include "DL_list_proc.h"
#include "DL_list_err_proc.h"
#include "general.h"

void fill_neighboring_idx(DL_list_node_t *node) {
    if (node->next == NULL) {
        node->next_idx = -1;
    } else {
        node->next_idx = node->next->idx;
    }
    if (node->prev == NULL) {
        node->prev_idx = -1;
    } else {
        node->prev_idx = node->prev->idx;
    }
}

bool DL_list_ctor(DL_list_t *list, const size_t size) {

    list->size = size + 2;
    list->data = (DL_list_node_t *) calloc((size_t) list->size, sizeof(DL_list_node_t));
    if (list->data == NULL) {
        DEBUG_DL_LIST_ERROR(DL_ERR_ALLOC, "")
        CLEAR_MEMORY(exit_mark);
    }

    list->data[0].empty = false;
    list->data[0].next = &list->data[0];
    list->data[0].prev = &list->data[0];

    list->data[0].idx = 0;
    fill_neighboring_idx(&list->data[0]);

    list->data[0].value = DL_LIST_POISON_VALUE;

    for (size_t i = 1; i < list->size; i++) {
        list->data[i].empty = true;
        list->data[i].next = NULL;
        list->data[i].prev = NULL;

        list->data[i].idx = (int) i;
        list->data[i].next_idx = -1;
        list->data[i].prev_idx = -1;
        fill_neighboring_idx(&list->data[i]);

        list->data[i].value = DL_LIST_POISON_VALUE;
    }

    list->free_node = &list->data[1];
    for (size_t i = 1; i < list->size - 1; i++) {
        list->data[i].next = &list->data[i + 1]; // creation of one linked free_nodes list
        list->data[i].next_idx = list->data[i + 1].idx;
    }
    return true;

    exit_mark:

    if (list->data != NULL) {
        FREE(list->data);
    }
    return false;
}


bool resize_up(DL_list_t *list) {
    assert(list != NULL);

    return false;

    // size_t old_size = list->size;
    // list->size *= resize_up_coeff;
    // list->data = (DL_list_node_t *) realloc(list->data, list->size * sizeof(DL_list_node_t));
    // printf("realloc: %lu\n", list->size);
    // if (list->data == NULL) {
    //     DEBUG_DL_LIST_ERROR(DL_ERR_REALLOC, "")
    //     return false;
    // }

    // // for (size_t i = 0; i < old_size; i++) {
    // //     printf("idx : [%d], ptr : [%p]\n", list->data[i].idx, &list->data[i]);
    // //     printf("prev_idx : [%d], next_idx : [%d]\n", list->data[i].prev_idx, list->data[i].next_idx);
    // //     printf("prev_ptr : [%p], next_ptr : [%p]\n\n", list->data[i].prev, list->data[i].next);
    // // }

    // list->free_node = &list->data[old_size - 1];

    // for (size_t i = 0; i < old_size; i++) {
    //     if (list->data[i].prev_idx != -1) {
    //         list->data[i].prev = &list->data[list->data[i].prev_idx];
    //     }
    //     if (list->data[i].next_idx != -1) {
    //         list->data[i].next = &list->data[list->data[i].next_idx];
    //     }
    // }

    // list->data[old_size - 1].next = &list->data[old_size];
    // list->data[old_size - 1].next_idx = (int) old_size;

    // for (size_t i = old_size; i < list->size; i++) {
    //     list->data[i].empty = true;
    //     list->data[i].next = NULL;
    //     list->data[i].prev = NULL;

    //     list->data[i].idx = (int) i;
    //     list->data[i].next_idx = -1;
    //     list->data[i].prev_idx = -1;
    //     fill_neighboring_idx(&list->data[i]);

    //     list->data[i].value = DL_LIST_POISON_VALUE;
    // }

    // for (size_t i = old_size; i < list->size - 1; i++) {
    //     list->data[i].next = &list->data[i + 1]; // creation of one linked free_nodes list
    //     list->data[i].next_idx = list->data[i + 1].idx;
    // }


    // return true;
}

void DL_list_dtor(DL_list_t *list) {
    assert(list != NULL);

    if (list->data != NULL) {
        FREE(list->data);
    }
}

DL_list_node_t *DL_list_get_free_cell_addr(DL_list_t *list) {
    assert(list != NULL);

    if (list->free_node->next == NULL) {
        resize_up(list);
        if (!resize_up(list)) {
            debug("resize_up failed");
            return NULL;
        }
    }
    DL_list_node_t *free_node = list->free_node;

    list->free_node = list->free_node->next;

    return free_node;
}

DL_list_elem_value_t *DL_list_front(DL_list_t *list) {
    DL_list_node_t *front_node = list->data[0].next;
    if (front_node->value == DL_LIST_POISON_VALUE) {
        return NULL;
    }
    return &front_node->value;
}

DL_list_elem_value_t *DL_list_back(DL_list_t *list) {
    DL_list_node_t *back_node = list->data[0].prev;
    if (back_node->value == DL_LIST_POISON_VALUE) {
        return NULL;
    }
    return &back_node->value;
}

DL_list_elem_value_t *DL_list_next_elem(DL_list_t *list, const int addr) {
    if (addr >= (int) list->size || addr < 0) {
        return NULL;
    }
    DL_list_node_t *next_node = list->data[addr].next;
    if (next_node->value == DL_LIST_POISON_VALUE) {
        return NULL;
    }
    return &next_node->value;
}

DL_list_elem_value_t *DL_list_prev_elem(DL_list_t *list, const int addr) {
    if (addr >= (int) list->size || addr < 0) {
        return NULL;
    }
    DL_list_node_t *prev_node = list->data[addr].prev;
    if (prev_node->value == DL_LIST_POISON_VALUE) {
        return NULL;
    }
    return &prev_node->value;
}

DL_list_node_t *DL_list_push_back(DL_list_t *list, const DL_list_elem_value_t value) {
    assert(list != NULL);
    // FIXME: если буфер переполняется, то просто возвращаем ошибку, не делаем recalloc

    DL_list_node_t *new_node = DL_list_get_free_cell_addr(list);
    if (new_node == NULL) {
        DEBUG_DL_LIST_ERROR(DL_ERR_PUSH, "DL_list_get_free_cell_addr: {-1}")
        return NULL;
    }

    DL_list_node_t *zero_node = &list->data[0];
    DL_list_node_t *old_last_node = zero_node->prev;

    new_node->empty = false;
    new_node->value = value;
    new_node->prev = old_last_node;
    new_node->next = zero_node;
    fill_neighboring_idx(new_node);

    old_last_node->next = new_node;
    fill_neighboring_idx(old_last_node);

    zero_node->prev = new_node;
    fill_neighboring_idx(zero_node);

    return new_node;
}

DL_list_node_t *DL_list_push_front(DL_list_t *list, const DL_list_elem_value_t value) {
    assert(list != NULL);

    DL_list_node_t *new_node = DL_list_get_free_cell_addr(list);
    if (new_node == NULL) {
        DEBUG_DL_LIST_ERROR(DL_ERR_PUSH, "DL_list_get_free_cell_addr: {-1}")
        return NULL;
    }

    DL_list_node_t *zero_node = &list->data[0];
    DL_list_node_t *old_first_node = zero_node->next;

    new_node->empty = false;
    new_node->value = value;
    new_node->next = old_first_node;
    new_node->prev = zero_node;
    fill_neighboring_idx(new_node);

    old_first_node->prev = new_node;
    fill_neighboring_idx(old_first_node);

    zero_node->next = new_node;
    fill_neighboring_idx(zero_node);

    return new_node;
}

DL_list_node_t *DL_list_insert_back(DL_list_t *list, DL_list_node_t *ptr, const DL_list_elem_value_t value) {
    assert(list != NULL);

    DL_list_node_t *new_node = DL_list_get_free_cell_addr(list);
    if (new_node == NULL) {
        DEBUG_DL_LIST_ERROR(DL_ERR_PUSH, "DL_list_get_free_cell_addr: {-1}")
        return NULL;
    }

    if (ptr == NULL) {
        DEBUG_DL_LIST_ERROR(DL_ERR_INSERT, "nullptr");
        return NULL;
    }



    DL_list_node_t *mid_node = ptr;
    DL_list_node_t *right_node = mid_node->next;

    new_node->empty = false;
    new_node->next = right_node;
    new_node->prev = mid_node;
    new_node->value = value;
    fill_neighboring_idx(new_node);

    mid_node->next = new_node;
    fill_neighboring_idx(mid_node);

    right_node->prev = new_node;
    fill_neighboring_idx(right_node);


    return new_node;
}

DL_list_node_t *DL_list_insert_front(DL_list_t *list, DL_list_node_t *ptr, const DL_list_elem_value_t value) {
    assert(list != NULL);

    DL_list_node_t *new_node = DL_list_get_free_cell_addr(list);
    if (new_node == NULL) {
        DEBUG_DL_LIST_ERROR(DL_ERR_PUSH, "DL_list_get_free_cell_addr: {-1}")
        return NULL;
    }

    if (ptr == NULL) {
        DEBUG_DL_LIST_ERROR(DL_ERR_INSERT, "nullptr");
        return NULL;
    }



    DL_list_node_t *mid_node = ptr;
    DL_list_node_t *left_node = mid_node->prev;

    new_node->empty = false;
    new_node->next = mid_node;
    new_node->prev = left_node;
    new_node->value = value;
    fill_neighboring_idx(new_node);

    mid_node->prev = new_node;
    fill_neighboring_idx(mid_node);

    left_node->next = new_node;
    fill_neighboring_idx(left_node);


    return new_node;
}

void DL_list_reset_node(DL_list_node_t *node) {
    node->next = NULL;
    node->prev = NULL;
    node->empty = true;
    fill_neighboring_idx(node);
    node->value = DL_LIST_POISON_VALUE;
}

bool DL_list_pop(DL_list_t *list, DL_list_node_t *ptr) {
    assert(list != NULL);

    if (ptr == NULL) {
        DEBUG_DL_LIST_ERROR(DL_ERR_INSERT, "nullptr");
        return false;
    }

    DL_list_node_t *mid_node = ptr;
    if (mid_node->next == NULL || mid_node->prev == NULL) {
        DEBUG_DL_LIST_ERROR(DL_ERR_POP, "nullptr");
        return false;
    }

    DL_list_node_t *left_node = mid_node->prev;
    DL_list_node_t *right_node = mid_node->next;


    left_node->next = right_node;
    fill_neighboring_idx(left_node);

    right_node->prev = left_node;
    fill_neighboring_idx(right_node);

    DL_list_reset_node(mid_node);
    mid_node->next = list->free_node;
    list->free_node = mid_node;

    return true;
}

DL_list_node_t *DL_list_find(DL_list_t *list, const DL_list_elem_value_t value) {
    assert(list != NULL);

    DL_list_node_t *node = &list->data[0];
    node = node->next;

    size_t iterations = 0;

    while (node != NULL) {
        if (iterations >= MAX_CYCLE_ITERATIONS) {
            DEBUG_DL_LIST_ERROR(DL_ERR_CYCLED, "")
            return NULL;
        }
        if (node->value == value) {
            return node;
        }
        if (node->next == NULL) {
            DEBUG_DL_LIST_ERROR(DL_ERR_INVALID_NODE,
                "reachable node has invalid parameters: addr: {%p}, next: {%p} prev: {%p}", node, node->next, node->prev)
            return NULL;
        }
        node = node->next;
        iterations++;
    }
    return NULL;
}

void DL_list_clear(DL_list_t *list) {
    assert(list != NULL);

    for (size_t i = 1; i < list->size; i++) {
        DL_list_reset_node(&list->data[i]);
    }
    list->data[0].next = &list->data[0];
    list->data[0].prev = &list->data[0];
}
