#ifndef ALGO_VIEW_H
#define ALGO_VIEW_H
#include "tigr.h"
#include <iostream>

#ifdef __cplusplus
extern "C"{
#endif

/*Just a set of decently visiable colors*/
//could use some looking over to make sure it will
//be accessable for color blind students
#define BACKGROUNDCOLOR tigrRGB(0x34, 0x80, 0x9e)
#define BARCOLOR tigrRGB(0xb6, 0xb9, 0xba)
#define COLOR1 tigrRGB(0x57, 0x04, 0x63)
#define COLOR2 tigrRGB(0xe6, 0x70, 0xc4)
#define COLOR3 tigrRGB(0x78, 0x0b, 0x05)
#define COLOR4 tigrRGB(0xf0, 0x25, 0x1a)
#define COLOR5 tigrRGB(0x01, 0x52, 0x08)
#define COLOR6 tigrRGB(0x41, 0xba, 0x4b)
#define COLOR7 tigrRGB(0xf7, 0xe7, 0x02)
#define COLOR8 tigrRGB(0xcf, 0x67, 0x06)

#define DEFAULT_KEYBOX_HEIGHT 25 //Height of the box that shows which color is which variable

#define DEFAULT_WINDOW_HEIGHT 800
#define DEFAULT_WINDOW_WIDTH 1000

/*Define algoView Object singleton for the state machine*/


/*Function to draw array will be called many times but the first of which
*   should set singleton object up for all the consequtive calls*/

/*Cleanup function should be called atexit() so student does not have to worry about cleanup*/


//function to signal last draw call that is going to happen essentailly used to signify that the algorithm being tested has run its course
#ifdef __cplusplus
}
#endif


#endif