#ifndef ALGO_VIEW_H
#define ALGO_VIEW_H
#include "tigr.h"
#include <iostream>

#define BACKGROUNDCOLOR {0x34, 0x80, 0x9e}
#define BARCOLOR {0xb6, 0xb9, 0xba}
#define COLOR1 {0x57, 0x04, 0x63}
#define COLOR2 {0xe6, 0x70, 0xc4}
#define COLOR3 {0x78, 0x0b, 0x05}
#define COLOR4 {0xf0, 0x25, 0x1a}
#define COLOR5 {0x01, 0x52, 0x08}
#define COLOR6 {0x41, 0xba, 0x4b}
#define COLOR7 {0xf7, 0xe7, 0x02}
#define COLOR8 {0xcf, 0x67, 0x06}

#define DEFAULT_KEYBOX_HEIGHT 25

#define DEFAULT_WINDOW_HEIGHT 800
#define DEFAULT_WINDOW_WIDTH 1000

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Color Color;
struct Color{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

const Color COLORS[8] = {COLOR1, COLOR2,COLOR3, COLOR4,COLOR5, COLOR6,COLOR7, COLOR8};

int colorCount = 0;

typedef struct Label Label;
struct Label{
    int index;
    char* label;
    TPixel color;
};

static Label label(int index, char* label, Color color = BACKGROUNDCOLOR){
    if(color.r == 0x34 && color.g == 0x80 && color.b == 0x9e){
        //color assumed not provided because it is the same as the background color
        //and would cause confusion if used anyway
    }
}


#ifdef __cplusplus
}
#endif

#endif