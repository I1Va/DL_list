#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdlib.h>

#include "DL_list_proc.h"
#include "DL_list_err_proc.h"
#include "general.h"

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
    list->data[0].value = DL_LIST_POISON_VALUE;

    for (size_t i = 1; i < list->size; i++) {
        list->data[i].empty = true;
        list->data[i].next = NULL;
        list->data[i].prev = NULL;
        list->data[i].value = DL_LIST_POISON_VALUE;
    }

    list->free_node = &list->data[1];
    for (size_t i = 1; i < list->size - 1; i++) {
        list->data[i].next = &list->data[i + 1]; // creation of one linked free_nodes list
    }
    return true;

    exit_mark:

    if (list->data != NULL) {
        FREE(list->data);
    }
    return false;
}

void DL_list_dtor(DL_list_t *list) {
    assert(list != NULL);

    if (list->data != NULL) {
        FREE(list->data);
    }
}

DL_list_node_t *DL_list_get_free_cell_addr(DL_list_t *list) {
    DL_list_node_t *free_node = list->free_node;
    if (list->free_node->next == NULL) {
        debug("list free nodes limit exceeded");
        return NULL;
    }

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
    if (new_node != NULL) {
        DEBUG_DL_LIST_ERROR(DL_ERR_PUSH, "DL_list_get_free_cell_addr: {-1}")
        return NULL;
    }

    DL_list_node_t *zero_node = &list->data[0];
    DL_list_node_t *old_last_node = zero_node->prev;

    new_node->empty = false;
    new_node->value = value;
    new_node->prev = old_last_node;
    new_node->next = zero_node;

    old_last_node->next = new_node;

    zero_node->prev = new_node;

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

    new_node->value = value;
    new_node->next = old_first_node;
    new_node->prev = zero_node;

    old_first_node->prev = new_node;

    zero_node->next = new_node;

    return new_node;
}

DL_list_node_t *DL_list_insert_back(DL_list_t *list, DL_list_node_t *ptr, const DL_list_elem_value_t value) { // FIXME: вместо addr исп. указатели
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

    new_node->value = value;

    DL_list_node_t *mid_node = ptr;
    DL_list_node_t *right_node = mid_node->next;


    new_node->next = right_node;
    new_node->prev = mid_node;
    new_node->empty = false;

    mid_node->next = new_node;
    right_node->prev = new_node;

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

    new_node->value = value;

    DL_list_node_t *mid_node = ptr;
    DL_list_node_t *left_node = mid_node->prev;

    new_node->next = mid_node;
    new_node->prev = left_node;
    new_node->empty = false;

    mid_node->prev = new_node;
    left_node->next = new_node;

    return new_node;
}

void DL_list_reset_node(DL_list_node_t *node) {
    node->next = NULL;
    node->prev = NULL;
    node->empty = true;
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
    right_node->prev = left_node;

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

// TODO: сделать realloc списка вверх

void DL_list_clear(DL_list_t *list) {
    assert(list != NULL);

    for (size_t i = 1; i < list->size; i++) {
        DL_list_reset_node(&list->data[i]);
    }
    list->data[0].next = 0;
    list->data[0].prev = 0;
}
