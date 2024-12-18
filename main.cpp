#include "DL_list_logger.h"
#include "DL_list_proc.h"
#include "general.h"
#include <cstdio>
#include <cstdlib>

const char LOGS_DIR[] = "./logs";
const char LOG_FILE[] = "./logs/logfile.html";

int main() {

    log_t log_obj = {};
    if (!logs_ctor(&log_obj, LOGS_DIR, LOG_FILE)) {
        debug("logs_ctor failed");
        return EXIT_FAILURE;
    }
    log_obj.short_log = false;

    // DL_list_err_t last_err = DL_ERR_OK;

    DL_list_t list = {};
    DL_list_ctor(&list, 1);

    for (int i = 0; i < 10; i++) {
        int val = rand() % 128;
        // printf("val[%d] : %d\n", i, val);
        DL_list_push_back(&list, val);
    }

    // resize_up(&list);



    graphviz_start_graph(log_obj.graph_log.graphviz_code_file);
    DL_list_generate_graph_dot(&list, &log_obj);
    // graphviz_fillcolor_node(log_obj.graph_log.graphviz_code_file, &list.data[4], "#FFE0E0");
    graphviz_end_graph(log_obj.graph_log.graphviz_code_file);
    ListLogDump(&list, &log_obj);



    // DL_list_insert_back(&list, &list.data[3], 2006);
    // DL_list_insert_back(&list, &list.data[2], 7500);

    // DL_list_pop(&list, &list.data[4]);

    // for (size_t i = 0; i < list.size; i++) {
    //     printf("[%2lu]: ", i);
    //     fprintf_node(stdout, &list.data[i]);
    // }

    // DL_list_insert_front(&list, &list.data[4], 7500);

    // DL_list_insert_back(&list, &list.data[6], 228);
    // DEBUG_DL_LIST_ERROR(DL_list_verify(&list), "")


    DL_list_dtor(&list);

    return 0;
}