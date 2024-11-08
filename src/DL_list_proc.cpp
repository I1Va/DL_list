#include <assert.h>
#include <stddef.h>
#include <cstdlib>
#include <stdlib.h>

#include "DL_list_proc.h"
#include "DL_list_err_proc.h"
#include "general.h"

bool DL_list_ctor(DL_list_t *list, const size_t size, const char log_path[] = "") {

    list->size = size + 2;
    list->data = (DL_list_node_t *) calloc((size_t) list->size, sizeof(DL_list_node_t));
    if (list->data == NULL) {
        DEBUG_DL_LIST_ERROR(DL_ERR_ALLOC, "")
        CLEAR_MEMORY(exit_mark);
    }

    list->data[0].empty = false;
    list->data[0].next = 0;
    list->data[0].prev = 0;
    list->data[0].addr = 0;
    list->data[0].value = DL_LIST_POISON_VALUE;

    for (size_t i = 1; i < list->size; i++) {
        list->data[i].empty = true;
        list->data[i].next = -1;
        list->data[i].prev = -1;
        list->data[i].addr = (int) i;
        list->data[i].value = DL_LIST_POISON_VALUE;
    }

    list->free_addr = 1;
    for (size_t i = 1; i < list->size - 1; i++) {
        list->data[i].next = (int) i + 1; // creation of one linked free_nodes list
    }

    strcpy(list->log_file_path, log_path);
    list->log_file_ptr = fopen(log_path, "a");
    if (list->log_file_ptr == NULL) {
        DEBUG_DL_LIST_ERROR(DL_ERR_FILE_OPEN, "")
        CLEAR_MEMORY(exit_mark);
    }
    setbuf(list->log_file_ptr, NULL); // disable buffering


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

int DL_list_get_free_cell_addr(DL_list_t *list) {
    int free_node_addr = list->free_addr;
    if (list->data[list->free_addr].next == -1) {
        debug("list free nodes limit exceeded");
        return -1;
    }

    list->free_addr = list->data[list->free_addr].next;

    return free_node_addr;
}

DL_list_elem_value_t *DL_list_front(DL_list_t *list) {
    DL_list_node_t *front_node = &list->data[list->data[0].next];
    if (front_node->value == DL_LIST_POISON_VALUE) {
        return NULL;
    }
    return &front_node->value;
}

DL_list_elem_value_t *DL_list_back(DL_list_t *list) {
    DL_list_node_t *back_node = &list->data[list->data[0].prev];
    if (back_node->value == DL_LIST_POISON_VALUE) {
        return NULL;
    }
    return &back_node->value;
}

DL_list_elem_value_t *DL_list_next_elem(DL_list_t *list, const int addr) {
    if (addr >= (int) list->size || addr < 0) {
        return NULL;
    }
    DL_list_node_t *next_node = &list->data[list->data[addr].next];
    if (next_node->value == DL_LIST_POISON_VALUE) {
        return NULL;
    }
    return &next_node->value;
}

DL_list_elem_value_t *DL_list_prev_elem(DL_list_t *list, const int addr) {
    if (addr >= (int) list->size || addr < 0) {
        return NULL;
    }
    DL_list_node_t *prev_node = &list->data[list->data[addr].prev];
    if (prev_node->value == DL_LIST_POISON_VALUE) {
        return NULL;
    }
    return &prev_node->value;
}

int DL_list_push_back(DL_list_t *list, const DL_list_elem_value_t value) {
    assert(list != NULL);
    // FIXME: если буфер переполняется, то просто возвращаем ошибку, не делаем recalloc

    int new_node_addr = DL_list_get_free_cell_addr(list);
    if (new_node_addr == -1) {
        DEBUG_DL_LIST_ERROR(DL_ERR_PUSH, "DL_list_get_free_cell_addr: {-1}")
        return -1;
    }

    DL_list_node_t *new_node = &list->data[new_node_addr];
    DL_list_node_t *zero_node = &list->data[0];
    DL_list_node_t *old_last_node = &list->data[zero_node->prev];

    new_node->empty = false;
    new_node->value = value;
    new_node->prev = old_last_node->addr;
    new_node->next = zero_node->addr;
    new_node->addr = new_node_addr;


    old_last_node->next = new_node->addr;

    zero_node->prev = new_node->addr;

    return new_node_addr;
}

int DL_list_push_front(DL_list_t *list, const DL_list_elem_value_t value) {
    assert(list != NULL);

    int new_node_addr = DL_list_get_free_cell_addr(list);
    if (new_node_addr == -1) {
        DEBUG_DL_LIST_ERROR(DL_ERR_PUSH, "DL_list_get_free_cell_addr: {-1}")
        return -1;
    }

    DL_list_node_t *new_node = &list->data[new_node_addr];
    DL_list_node_t *zero_node = &list->data[0];
    DL_list_node_t *old_first_node = &list->data[zero_node->next];

    new_node->value = value;
    new_node->next = old_first_node->addr;
    new_node->prev = zero_node->addr;
    new_node->addr = new_node_addr;


    old_first_node->prev = new_node->addr;

    zero_node->next = new_node->addr;

    return new_node_addr;
}

int DL_list_insert_back(DL_list_t *list, const int addr, const DL_list_elem_value_t value) { // FIXME: вместо addr исп. указатели
    assert(list != NULL);

    int new_node_addr = DL_list_get_free_cell_addr(list);
    if (new_node_addr == -1) {
        DEBUG_DL_LIST_ERROR(DL_ERR_PUSH, "DL_list_get_free_cell_addr: {-1}")
        return -1;
    }

    if (addr >= (int) list->size || addr < 0) {
        DEBUG_DL_LIST_ERROR(DL_ERR_INSERT, "invalid addr: {%d}", addr);
        return -1;
    }

    DL_list_node_t *new_node = &list->data[new_node_addr];
    new_node->value = value;
    new_node->addr = new_node_addr;

    DL_list_node_t *mid_node = &list->data[addr];
    DL_list_node_t *right_node = &list->data[mid_node->next];


    new_node->next = right_node->addr;
    new_node->prev = mid_node->addr;
    new_node->empty = false;

    mid_node->next = new_node->addr;
    right_node->prev = new_node->addr;

    return new_node_addr;
}

int DL_list_insert_front(DL_list_t *list, const int addr, const DL_list_elem_value_t value) {
    assert(list != NULL);

    int new_node_addr = DL_list_get_free_cell_addr(list);
    if (new_node_addr == -1) {
        DEBUG_DL_LIST_ERROR(DL_ERR_PUSH, "DL_list_get_free_cell_addr: {-1}")
        return -1;
    }

    if (addr >= (int) list->size || addr < 0) {
        DEBUG_DL_LIST_ERROR(DL_ERR_INSERT, "invalid addr: {%d}", addr);
        return -1;
    }

    DL_list_node_t *new_node = &list->data[new_node_addr];
    new_node->value = value;
    new_node->addr = new_node_addr;

    DL_list_node_t *mid_node = &list->data[addr];
    DL_list_node_t *left_node = &list->data[mid_node->prev];

    new_node->next = mid_node->addr;
    new_node->prev = left_node->addr;
    new_node->empty = false;

    mid_node->prev = new_node->addr;
    left_node->next = new_node->addr;

    return new_node_addr;
}

void DL_list_reset_node(DL_list_node_t *node) {
    node->next = -1;
    node->prev = -1;
    node->empty = true;
    node->value = DL_LIST_POISON_VALUE;
}

bool DL_list_pop(DL_list_t *list, const int addr) {
    assert(list != NULL);

    if (addr >= (int) list->size || addr < 0 || addr == 0) {
        DEBUG_DL_LIST_ERROR(DL_ERR_INSERT, "invalid addr: {%d}", addr);
        return false;
    }

    DL_list_node_t *mid_node = &list->data[addr];
    if (mid_node->next == -1 || mid_node->prev == -1) {
        DEBUG_DL_LIST_ERROR(DL_ERR_POP, "invalid addr: {%d}", addr);
        return false;
    }

    DL_list_node_t *left_node = &list->data[mid_node->prev];
    DL_list_node_t *right_node = &list->data[mid_node->next];


    left_node->next = right_node->addr;
    right_node->prev = left_node->addr;

    DL_list_reset_node(mid_node);
    mid_node->next = list->free_addr;
    list->free_addr = mid_node->addr;

    return true;
}

int DL_list_find(DL_list_t *list, const DL_list_elem_value_t value) {
    assert(list != NULL);

    DL_list_node_t node = list->data[0];
    node = list->data[node.next];

    size_t iterations = 0;

    while (node.addr != 0) {
        if (iterations >= MAX_CYCLE_ITERATIONS) {
            DEBUG_DL_LIST_ERROR(DL_ERR_CYCLED, "")
            return -1;
        }
        if (node.value == value) {
            return node.addr;
        }
        if (node.next == -1) {
            DEBUG_DL_LIST_ERROR(DL_ERR_INVALID_NODE,
                "reachable node has invalid parameters: addr: {%d}, next: {%d} prev: {%d}", node.addr, node.next, node.prev)
            return -1;
        }
        node = list->data[node.next];
        iterations++;
    }
    return -1;
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
