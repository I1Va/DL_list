#include "DL_list_err_proc.h"
#include "DL_list_logger.h"
#include "DL_list_proc.h"
#include "general.h"
#include <cstdio>
#include <cstdlib>

const char logs_dir[] = "./logs";

int main() {
    create_logs_dir(logs_dir);

    // DL_list_err_t last_err = DL_ERR_OK;

    DL_list_t list = {};
    DL_list_ctor(&list, 10, "./logs/log.html");
    DL_list_log_file_start(list.log_file_ptr);

    for (int i = 0; i < 7; i++) {
        DL_list_push_back(&list, rand() % 128);
    }

    DL_list_pop(&list, 4);
    DL_list_insert_back(&list, 3, 2006);

    DL_list_insert_front(&list, 4, 7500);

    DL_list_insert_back(&list, 6, 228);

    DEBUG_DL_LIST_ERROR(DL_list_verify(list), "")
    ListLogDump(&list); // FIXME: можно передавать указатель на logfile и не хранить в list

    // DL_list_insert(&list, 4, 52, &last_err);

    DL_list_dtor(&list);

    return 0;
}