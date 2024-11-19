#ifndef DL_LIST_LOGGER_H
#define DL_LIST_LOGGER_H

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <stdarg.h>

#include "DL_list_proc.h"


const char EMPTY_NODE_COLOR[] = "blue";
const char FICTIVE_NODE_COLOR[] = "purple";

const int EDGE_MAX_WEIGHT = 1024;
const int SIMP_EDGE_WIDTH = 2;

const size_t BORDER_SZ = 100;
const size_t LOG_WIDTH_VAL = 100;

enum DL_list_log_type_t {
    DL_LOG_ANALYS = 0,
    DL_LOG_DEBUG = 1,
    DL_LOG_ERROR = 2,
};

struct graphviz_dir_t {
    char log_dir[MAX_LOG_DIR_SZ];
    char img_dir[MAX_IMG_DIR_SZ];
    char graphviz_codes_dir[MAX_GRAPHVIZ_CODES_DIR_SZ];
    char short_img_path[MAX_LOG_FILE_PATH_SZ];

    char graphviz_code_file_name[MAX_LOG_FILE_PATH_SZ];
    int graph_num;

    char img_file_name[MAX_LOG_FILE_PATH_SZ] = {};

    FILE* graphviz_code_file;

};

struct log_t {
    char log_dir[MAX_LOG_DIR_SZ];
    char log_file[MAX_LOG_FILE_PATH_SZ];


    bool short_log;

    FILE *log_file_ptr;

    graphviz_dir_t graph_log;
};


void graphviz_make_list_same_rank(FILE *graphviz_code_file, DL_list_t *list);
bool create_logs_dir(const char log_dir[]);
bool logs_ctor(log_t *log_obj, const char log_dir[], const char log_file[]);
void DL_list_fprintf_border(FILE* stream, const char bord_char, const size_t bord_sz, bool new_line);
void DL_list_fprintf_title(FILE *stream, const char tittle[], const char bord_char, const size_t bord_sz);
void graphviz_make_tiny_node(FILE *graphviz_code_file, DL_list_node_t *node);
const char *DL_list_get_log_descr(enum DL_list_log_type_t log_type);
void DL_list_log_print_time(FILE *log_output_file_ptr);
void DL_list_print_log_func_info(FILE *log_output_file_ptr, const char file_name[], const char func_name[], const int line_idx);
void DL_list_print_log_type(FILE *log_output_file_ptr, enum DL_list_log_type_t log_type);
void DL_list_log_var_print(FILE *log_output_file_ptr, enum DL_list_log_type_t log_type, const char file_name[], const char func_name[], const int line_idx, const char fmt[], ...);
void DL_list_log_dump(DL_list_t *list, log_t *log_obj, const char file_name[], const char func_name[], const int line_idx);
int get_dir_files_count(const char dir_path[]);
graphviz_dir_t DL_list_make_graphviz_dirs(char log_file_path[]);
void graphviz_start_graph(FILE *graphviz_code_file);
void graphviz_end_graph(FILE *graphviz_code_file);
void graphviz_make_node(FILE *graphviz_code_file, DL_list_node_t *node);
void graphviz_make_heavy_unvisible_edge(FILE *graphviz_code_file, DL_list_node_t *node1, DL_list_node_t *node2);
void graphviz_make_edge(FILE *graphviz_code_file, DL_list_node_t *node1, DL_list_node_t *node2, const char color[], int penwidth);
void DL_list_log_html_insert_image(FILE *log_output_file_ptr, char short_img_path[], int width_percent);
bool DL_list_generate_graph_dot(DL_list_t *list, log_t *log_obj);
void graphviz_color_node(FILE *graphviz_code_file, DL_list_node_t *node, const char color[]);

#define ListLogVar(log_output_file_ptr, log_type, fmt, ...) log_var_print(log_output_file_ptr, log_type, __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__, fmt, ##__VA_ARGS__);

#define ListLogDump(list, logs_obj_ptr) DL_list_log_dump(list, logs_obj_ptr, __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__);

#endif // DL_LIST_LOGGER_H