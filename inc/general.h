#ifndef GENERAL_H
#define GENERAL_H

#include <stddef.h>
#include <time.h>
#include <stdio.h>


const size_t date_nmemb = 16;
const size_t time_nmemb = 16;

#define GRN "\e[0;32m"
#define WHT "\e[0;20m"
#define RED "\e[0;31m"
#define YEL "\e[0;33m"

#define HTML_GRN "<font color=\"green\">"
#define HTML_BLACK "<font color=\"black\">"
#define HTML_RED "<font color=\"red\">"
#define HTML_YEL "<font color=\"yellow\">"

#define printf_red(str_, ...) printf(RED str_ WHT, ##__VA_ARGS__)
#define printf_wht(str_, ...) printf(WHT str_ WHT, ##__VA_ARGS__)
#define printf_grn(str_, ...) printf(GRN str_ WHT, ##__VA_ARGS__)
#define printf_yel(str_, ...) printf(YEL str_ WHT, ##__VA_ARGS__)


#define fprintf_red(stream, str_, ...) fprintf(stream, RED str_, ##__VA_ARGS__)
#define fprintf_wht(stream, str_, ...) fprintf(stream, WHT str_, ##__VA_ARGS__)
#define fprintf_grn(stream, str_, ...) fprintf(stream, GRN str_, ##__VA_ARGS__)
#define fprintf_yel(stream, str_, ...) fprintf(stream, YEL str_, ##__VA_ARGS__)


#define fprintf_html_red(stream, str_, ...) fprintf(stream, HTML_RED str_, ##__VA_ARGS__)
#define fprintf_html_grn(stream, str_, ...) fprintf(stream, HTML_GRN str_, ##__VA_ARGS__)
#define fprintf_html_black(stream, str_, ...) fprintf(stream, HTML_BLACK str_, ##__VA_ARGS__)
#define fprintf_html_yel(stream, str_, ...) fprintf(stream, HTML_YEL str_, ##__VA_ARGS__)

#define FREE(ptr)              \
    do {                       \
        free(ptr); ptr = NULL; \
    } while(0);


/*
#define FREE(ptr)                \
    ({                           \
        typeof(ptr) ptr__ = ptr; \
        free(ptr);               \
        ptr = NULL;              \
        ptr__                    \
    })
*/

enum RETURN_STATES {
    ReturnErrOR  = -1,
    RETURN_FALSE =  0,
    RETURN_TRUE  =  1,
};

#define ASSERT(error, end_instruction)                                                                                    \
    fprintf_red(stderr, "{%s} [%s: %d]: descr{%s}\n", __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__, stkerr_get_descr(error));    \
    end_instruction;

#define CLEAR_MEMORY(mark) goto mark;

#ifdef _DEBUG
    #define debug(str_, ...) fprintf_red(stderr, "{%s} [%s: %d]: descr{" str_ "}\n", __FILE_NAME__, __PRETTY_FUNCTION__, __LINE__, ##__VA_ARGS__)
    #define ON_DEBUG(...) __VA_ARGS__
    #define MY_ASSERT(stk_err, exit_instruction) {DEBUG_STK_ERROR(stk_err); exit_instruction;};

#else
    #define debug(str_, ...) ;
    #define ON_DEBUG(...)
    #define MY_ASSERT(stk_err, exit_instruction) ;

#endif // _DEBUG

#endif // GENERAL
