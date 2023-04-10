#ifndef ALGO_VIEW_H
#define ALGO_VIEW_H
#include "tigr.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#define DEFAULT_HISTORY_SIZE 100

static const TPixel AV_COLORS[8] = {COLOR1, COLOR2, COLOR3, COLOR4, COLOR5, COLOR6, COLOR7, COLOR8};

static void inline av_cleanup();

/*Define algoView Object singleton for the state machine*/
struct AlgoView{
    unsigned int windowWidth;
    unsigned int windowHeight;
    unsigned int keyboxHeight;
    Tigr *screen = NULL;
    int **history;
    int historySize;
    int currentSize;
};

static struct AlgoView* av_view;

static void av_drawArrayUtility(int array[], int arraySize, int markedSize, int markedIndexes[], std::string indexAlias[]){
    //calculating variables that are usful for drawing the array
    int width = av_view->windowWidth/arraySize;
    tigrClear(av_view->screen, BACKGROUNDCOLOR);
    tigrRect(av_view->screen, 0, av_view->windowHeight - av_view->keyboxHeight, (int)av_view->windowWidth, (int)av_view->keyboxHeight, tigrRGB(0xFF, 0xFF, 0xFF));
    for(int i = 0; i < arraySize; i++){
        TPixel c = tigrRGB(0x42, 0xFF, 0xFF);
        for(int x = 0; x < markedSize; x++){
            if(i == markedIndexes[x])
                c = AV_COLORS[x];
            tigrPrint(av_view->screen, tfont, x * av_view->windowWidth/markedSize + 10, av_view->windowHeight - (av_view->keyboxHeight*2)/3, AV_COLORS[x], indexAlias[x].c_str());
        }
        tigrFillRect(av_view->screen, width * i, av_view->windowHeight - av_view->keyboxHeight - 10*array[i], width, 10*array[i], c);
    }
}

/*Function to draw array will be called many times but the first of which
*   should set singleton object up for all the consequtive calls*/

//want call to look like draw(array, ARRAY_SIZE, {3, "p"}, {1, "l"}, {7, "h"});
//optional may include settings like keybox height and width and height of the window
static void inline av_draw(int array[], int arraySize, int markedIndexes[] = NULL,\
 std::string indexAlias[] = NULL, int markedSize = 0, unsigned int keyboxHeight = DEFAULT_KEYBOX_HEIGHT,\
  unsigned int windowWidth = DEFAULT_WINDOW_WIDTH, unsigned int windowHeight = DEFAULT_WINDOW_HEIGHT){
    printf("In the draw function\n");
    //Singelton design pattern for av_view holds state information for drawing algorithm
    if(av_view == NULL){
        av_view = (struct AlgoView*)malloc(sizeof(struct AlgoView));
        av_view->screen = tigrWindow(windowWidth, windowHeight, "AlgoView", TIGR_FIXED);
        atexit(av_cleanup);
        av_view->windowHeight = windowHeight;
        av_view->windowWidth = windowWidth;
        av_view->keyboxHeight = keyboxHeight;
        av_view->historySize = DEFAULT_HISTORY_SIZE;
        av_view->history = (int**)malloc(sizeof(int*) * av_view->historySize);
        av_view->currentSize = 1;
        av_view->history[0] = (int*)malloc(sizeof(int) * arraySize);
        memcpy(av_view->history[0], array, arraySize * sizeof(int));
    }
    else{
        if(av_view->currentSize == av_view->historySize)
        {
            av_view->historySize += 100;
            av_view->history = (int**)realloc(av_view->history, av_view->historySize * sizeof(int*));
        }
        av_view->history[av_view->currentSize] = (int*)malloc(sizeof(int) * arraySize);
        memcpy(av_view->history[av_view->currentSize], array, arraySize * sizeof(int));
        av_view->currentSize = av_view->currentSize+1;
    }
    int currentView = av_view->currentSize - 1;
    av_drawArrayUtility(array, arraySize, markedSize, markedIndexes, indexAlias);
    bool nextItteration = false;
    while(!nextItteration){
        if(tigrKeyDown(av_view->screen, 'D')){
            nextItteration = currentView == av_view->currentSize - 1;
            if(!nextItteration){
                currentView += 1;
                av_drawArrayUtility(av_view->history[currentView], arraySize, markedSize, markedIndexes, indexAlias);
            }
        }
        else if(tigrKeyDown(av_view->screen, 'A') && currentView > 0){
            currentView -= 1;
            av_drawArrayUtility(av_view->history[currentView], arraySize, markedSize, markedIndexes, indexAlias);
        }
        tigrUpdate(av_view->screen);
    }
}

static void inline av_end(int array[], int arraySize){
    //calculating variables that are usful for drawing the array
    int width = av_view->windowWidth/arraySize;
    tigrClear(av_view->screen, BACKGROUNDCOLOR);
    tigrRect(av_view->screen, 0, av_view->windowHeight - av_view->keyboxHeight, (int)av_view->windowWidth, (int)av_view->keyboxHeight, tigrRGB(0xFF, 0xFF, 0xFF));
    for(int i = 0; i < arraySize; i++){
        TPixel c = tigrRGB(0x42, 0xFF, 0xFF);
        tigrFillRect(av_view->screen, width * i, av_view->windowHeight - av_view->keyboxHeight - 10*array[i], width, 10*array[i], c);
    }
    bool keyDown = false;
    while(!keyDown){
        keyDown = tigrKeyDown(av_view->screen, 'D');
        tigrUpdate(av_view->screen);
    }
}

/*Cleanup function should be called atexit() so student does not have to worry about cleanup*/
static void inline av_cleanup(){
    tigrFree(av_view->screen);
    for(int i = 0; i < av_view->currentSize; i++){
        free(av_view->history[i]);
    }
    free(av_view->history);
    free(av_view);
}

//function to signal last draw call that is going to happen essentailly used to signify that the algorithm being tested has run its course
#ifdef __cplusplus
}
#endif


#endif