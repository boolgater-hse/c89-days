#ifndef __TOOLS_H__
#define __TOOLS_H__

#include "CEcoLab1.h"

#ifdef WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#endif

#define RED_NODE "\x1b[31;49m"
#define BLACK_NODE "\x1b[30;47m"
#define HIGHLIGHTED_NODE "\x1b[43m"
#define SUCCESS_NODE "\x1b[42m"
#define YELLOW "\x1b[33;49m"
#define DEFAULTS "\x1b[0m"

#define CLEAR_FROM_CRS_TO_BOTTOM "\x1b[0J"
#define SAVE_CRS_POS "\x1b[s"
#define RESTORE_CRS_POS "\x1b[u"
#define CRS_TO_HOME "\x1b[H"

#define BOLD "\x1b[1m"

#define NEXT_KEY 'n'
#define WAIT_FOR_NEXT                  \
do {                                   \
    int c = getch_();                  \
    while (c != NEXT_KEY && c != '\r'  \
           && c != '\n' && c != EOF) { \
        c = getch_();                  \
    }                                  \
} while (0)

void sleep_(int32_t millis);

int getch_(void);

#endif /* __TOOLS_H__ */
