#ifndef DRAW_H
#define DRAW_H

#include <windows.h>

typedef struct {
    POINT start;
    POINT end;
    COLORREF colour;
    int thickness;
} LINE;



#endif