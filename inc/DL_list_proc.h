#ifndef DL_PROC_H
#define DL_PROC_H

#include <string.h>
#include <stdio.h>

typedef int DL_list_elem_value_t;

struct DL_list_node_t {
    DL_list_node_t *next;
    DL_list_node_t *prev;

    int prev_idx;
    int next_idx;
    int idx;

    DL_list_elem_value_t value;
    bool empty;
};

const char LOG_IMG_DIR_NAME[] = "imgs";
const char LOG_GRAPHVIZ_CODE_DIR_NAME[] = "graphviz_code_dir";


const size_t MAX_FMT_SZ = 32;
const size_t MAX_LOG_DIR_SZ = 128;
const size_t MAX_IMG_DIR_SZ = MAX_LOG_DIR_SZ + MAX_FMT_SZ;
const size_t MAX_DIGITS_N = 32;
const size_t MAX_GRAPHVIZ_CODES_DIR_SZ = MAX_LOG_DIR_SZ + sizeof(LOG_GRAPHVIZ_CODE_DIR_NAME);
const size_t MAX_LOG_FILE_PATH_SZ = MAX_GRAPHVIZ_CODES_DIR_SZ + MAX_FMT_SZ;
const size_t MAX_SYSTEM_COMMAND_SIZE = MAX_GRAPHVIZ_CODES_DIR_SZ + MAX_IMG_DIR_SZ + MAX_FMT_SZ + 2 *  MAX_LOG_FILE_PATH_SZ;

const int resize_up_coeff = 2;

struct DL_list_t {
    size_t size;
    DL_list_node_t *free_node;
    DL_list_node_t *data;
};

const DL_list_elem_value_t DL_LIST_POISON_VALUE = 0xBADBAD;

DL_list_elem_value_t *DL_list_front(DL_list_t *list);
DL_list_elem_value_t *DL_list_back(DL_list_t *list);
void fill_neighboring_idx(DL_list_node_t *node);
DL_list_elem_value_t *DL_list_next_elem(DL_list_t *list, const int addr);
DL_list_elem_value_t *DL_list_prev_elem(DL_list_t *list, const int addr);
DL_list_node_t *DL_list_push_back(DL_list_t *list, const DL_list_elem_value_t value);
DL_list_node_t *DL_list_push_front(DL_list_t *list, const DL_list_elem_value_t value);
DL_list_node_t *DL_list_insert_back(DL_list_t *list, DL_list_node_t *ptr, const DL_list_elem_value_t value);
DL_list_node_t *DL_list_insert_front(DL_list_t *list, DL_list_node_t *ptr, const DL_list_elem_value_t value);
DL_list_node_t *DL_list_get_free_cell_addr(DL_list_t *list);
bool DL_list_ctor(DL_list_t *list, const size_t size);
void DL_list_dtor(DL_list_t *list);
bool DL_list_pop(DL_list_t *list, DL_list_node_t *ptr);
DL_list_node_t *DL_list_find(DL_list_t *list, const DL_list_elem_value_t value);
void DL_list_clear(DL_list_t *list);
void DL_list_reset_node(DL_list_node_t *node);
bool resize_up(DL_list_t *list);

#endif // DL_PROC_H