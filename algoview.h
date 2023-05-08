#ifndef ALGO_VIEW_H
#define ALGO_VIEW_H
#include "tigr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdbool.h>

/*Just a set of decently visiable colors*/
//could use some looking over to make sure it will
//be accessable for color blind students
TPixel BACKGROUNDCOLOR;
TPixel BARCOLOR;

#define DEFAULT_KEYBOX_HEIGHT 25 //Height of the box that shows which color is which variable

#define DEFAULT_WINDOW_HEIGHT 800
#define DEFAULT_WINDOW_WIDTH 1000

#define DEFAULT_HISTORY_SIZE 100
#define MAX_ALIAS_LENGTH 32

//serialization format
const char* HISTORY_FORMAT_OUT = "(%s, %d, %c, %f, %e)\n";

TPixel AV_COLORS[8];

static void av_cleanup();

static void av_serializeAndSave(const char* filename);
static void av_serializeAndSaveDefaultFile();

typedef struct{
    int *array;
    int arraySize;
    int *markedIndexes;
    const char **indexAliases;
    int markedSize;
}AV_HistoryItem;

/*Define algoView Object singleton for the state machine*/
struct AlgoView{
    unsigned int windowWidth;
    unsigned int windowHeight;
    unsigned int keyboxHeight;
    Tigr *screen;
    AV_HistoryItem *history;
    int historySize;
    int currentSize;
};

static struct AlgoView* av_view;

static void av_initialization(){
    BACKGROUNDCOLOR = tigrRGB(0x34, 0x80, 0x9e);
    BARCOLOR = tigrRGB(0xb6, 0xb9, 0xba);
    AV_COLORS[0] = tigrRGB(0x57, 0x04, 0x63);
    AV_COLORS[1] = tigrRGB(0xe6, 0x70, 0xc4);
    AV_COLORS[2] = tigrRGB(0x78, 0x0b, 0x05);
    AV_COLORS[3] = tigrRGB(0xf0, 0x25, 0x1a);
    AV_COLORS[4] = tigrRGB(0x01, 0x52, 0x08);
    AV_COLORS[5] = tigrRGB(0x41, 0xba, 0x4b);
    AV_COLORS[6] = tigrRGB(0xf7, 0xe7, 0x02);
    AV_COLORS[7] = tigrRGB(0xcf, 0x67, 0x06);
}

/**
 * @brief Appends a new history item to the av_view.
 *
 * This function creates a new AV_HistoryItem with the given input parameters,
 * and appends it to the singleton av_view history. Memory for the new history item is
 * allocated and must be freed when it is no longer needed with the use of av_cleanup
 * av_cleanup is automatically called atexit().
 *
 * @param array A constant pointer to an array of integers representing the data to be stored in the history item.
 * @param arraySize The size of the array parameter.
 * @param markedSize The number of marked indexes in the history item.
 * @param markedIndexes A constant pointer to an array of integers representing the marked indexes.
 * @param indexAlias A constant pointer to an array of C-style strings representing the index aliases.
 */
static void av_appendHistory(const int array[], int arraySize, int markedSize, const int markedIndexes[], const char *indexAlias[]) {
    av_view->history[av_view->currentSize].array = (int *)malloc(sizeof(int) * arraySize);
    if (!av_view->history[av_view->currentSize].array) {
        // Handle memory allocation failure
        return;
    }
    memcpy(av_view->history[av_view->currentSize].array, array, arraySize * sizeof(int));
    av_view->history[av_view->currentSize].arraySize = arraySize;

    av_view->history[av_view->currentSize].indexAliases = (const char **)malloc(sizeof(const char *) * markedSize);
    if (!av_view->history[av_view->currentSize].indexAliases) {
        // Handle memory allocation failure
        free(av_view->history[av_view->currentSize].array);
        return;
    }
    for (int i = 0; i < markedSize; i++) {
        av_view->history[av_view->currentSize].indexAliases[i] = indexAlias[i];
    }

    av_view->history[av_view->currentSize].markedIndexes = (int *)malloc(sizeof(int) * markedSize);
    if (!av_view->history[av_view->currentSize].markedIndexes) {
        // Handle memory allocation failure
        free(av_view->history[av_view->currentSize].indexAliases);
        free(av_view->history[av_view->currentSize].array);
        return;
    }
    memcpy(av_view->history[av_view->currentSize].markedIndexes, markedIndexes, sizeof(int) * markedSize);
    av_view->history[av_view->currentSize].markedSize = markedSize;
    av_view->currentSize = av_view->currentSize + 1;
}

/**
 * @brief Draws one frame of the array visualization on the screen with optional marked elements and index aliases.
 *
 * This function takes an array, its size, and optional marked elements and index aliases
 * to draw a visualization of the array on the screen. The marked elements are drawn with
 * different colors and their corresponding index aliases are printed on the screen.
 *
 * @param array A constant pointer to an array of integers representing the data to be drawn.
 * @param arraySize The size of the array parameter.
 * @param markedSize The number of marked indexes in the array (default is 0).
 * @param markedIndexes A constant pointer to an array of integers representing the marked indexes (default is NULL).
 * @param indexAlias A constant pointer to an array of C-style strings representing the index aliases (default is NULL).
 */
static void av_drawArrayUtility(const int array[], int arraySize, int markedSize, const int markedIndexes[], const char* indexAlias[]) {
    // Calculating variables that are useful for drawing the array
    int width = av_view->windowWidth / arraySize;
    tigrClear(av_view->screen, BACKGROUNDCOLOR);
    tigrRect(av_view->screen, 0, av_view->windowHeight - av_view->keyboxHeight, (int)av_view->windowWidth, (int)av_view->keyboxHeight, tigrRGB(0xFF, 0xFF, 0xFF));
    
    for (int i = 0; i < arraySize; i++) {
        TPixel c = tigrRGB(0x42, 0xFF, 0xFF);
        
        for (int x = 0; x < markedSize; x++) {
            if (i == markedIndexes[x]) {
                c = AV_COLORS[x];
            }
            if (indexAlias) {
                tigrPrint(av_view->screen, tfont, x * av_view->windowWidth / markedSize + 10, av_view->windowHeight - (av_view->keyboxHeight * 2) / 3, AV_COLORS[x], indexAlias[x]);
            }
        }
        
        tigrFillRect(av_view->screen, width * i, av_view->windowHeight - av_view->keyboxHeight - 10 * array[i], width, 10 * array[i], c);
    }
}

/**
 * @brief Draws one frame of the array visualization on the screen with optional marked elements and index aliases.
 *
 * This function takes an array, its size, and optional marked elements and index aliases
 * to draw a visualization of the array on the screen. The marked elements are drawn with
 * different colors and their corresponding index aliases are printed on the screen.
 *
 * @param array A constant pointer to an array of integers representing the data to be drawn.
 * @param arraySize The size of the array parameter.
 */
static void av_drawArrayUtilityWithDefaults(int array[], int arraySize) {
    av_drawArrayUtility(array, arraySize, 0, NULL, NULL);
}

/**
 * @brief Draws the array visualization on the screen with optional marked elements and index aliases keeping screen updated.
 *
 * This function takes an array, its size, and optional marked elements and index aliases
 * to draw a visualization of the array on the screen. The marked elements are drawn with
 * different colors and their corresponding index aliases are printed on the screen.
 * The function also initializes the av_view singleton object if it has not been initialized yet.
 * Sets atexit to clean up memory allocated by the library and loops keeping window open and displaying
 * either old history on request or continues execution to next itteration on request from user.
 *
 * @param array A pointer to an array of integers representing the data to be drawn.
 * @param arraySize The size of the array parameter.
 * @param markedIndexes A pointer to an array of integers representing the marked indexes (default is NULL).
 * @param indexAlias A pointer to an array of C-style strings representing the index aliases (default is NULL).
 * @param markedSize The number of marked indexes in the array (default is 0).
 * @param keyboxHeight The height of the keybox area on the screen (default is DEFAULT_KEYBOX_HEIGHT).
 * @param windowWidth The width of the window (default is DEFAULT_WINDOW_WIDTH).
 * @param windowHeight The height of the window (default is DEFAULT_WINDOW_HEIGHT).
 */
static void av_drawWithSizes(int array[], int arraySize, int markedIndexes[], const char * indexAlias[], int markedSize, unsigned int keyboxHeight, unsigned int windowWidth, unsigned int windowHeight) {
    //Singelton design pattern for av_view holds state information for drawing algorithm
        if (av_view == NULL) {
            av_initialization();
            av_view = (struct AlgoView *)malloc(sizeof(struct AlgoView));
            av_view->screen = tigrWindow(windowWidth, windowHeight, "AlgoView", TIGR_FIXED);
            atexit(av_cleanup);
            av_view->windowHeight = windowHeight;
            av_view->windowWidth = windowWidth;
            av_view->keyboxHeight = keyboxHeight;
            av_view->historySize = DEFAULT_HISTORY_SIZE;
            av_view->history = (AV_HistoryItem *)malloc(sizeof(AV_HistoryItem) * av_view->historySize);
            av_view->currentSize = 1;
            av_view->history[0].array = (int *)malloc(sizeof(int) * arraySize);
            memcpy(av_view->history[0].array, array, arraySize * sizeof(int));
            av_view->history[0].arraySize = arraySize;
            av_view->history[0].indexAliases = (const char **)malloc(sizeof(const char *) * markedSize);
            for (int i = 0; i < markedSize; i++) {
                av_view->history[0].indexAliases[i] = strdup(indexAlias[i]);
            }
            av_view->history[0].markedIndexes = (int *)malloc(sizeof(int) * markedSize);
            memcpy(av_view->history[0].markedIndexes, markedIndexes, sizeof(int) * markedSize);
            av_view->history[0].markedSize = markedSize;
        } else {
            if (av_view->currentSize == av_view->historySize) {
                av_view->historySize += 100;
                av_view->history = (AV_HistoryItem *)realloc(av_view->history, av_view->historySize * sizeof(AV_HistoryItem));
            }
            av_appendHistory(array, arraySize, markedSize, markedIndexes, (const char **)indexAlias);
        }
    int currentView = av_view->currentSize - 1;
    av_drawArrayUtility(array, arraySize, markedSize, markedIndexes, indexAlias);
    bool nextItteration = false;
    while(!nextItteration){
        if(tigrKeyDown(av_view->screen, 'D')){
            nextItteration = currentView == av_view->currentSize - 1;
            if(!nextItteration){
                currentView += 1;
                av_drawArrayUtility(av_view->history[currentView].array, av_view->history[currentView].arraySize, av_view->history[currentView].markedSize, av_view->history[currentView].markedIndexes, av_view->history[currentView].indexAliases);
            }
        }
        else if(tigrKeyDown(av_view->screen, 'A') && currentView > 0){
            currentView -= 1;
                av_drawArrayUtility(av_view->history[currentView].array, av_view->history[currentView].arraySize, av_view->history[currentView].markedSize, av_view->history[currentView].markedIndexes, av_view->history[currentView].indexAliases);
        }
        tigrUpdate(av_view->screen);
    }
}

/**
 * @brief Draws the array visualization on the screen with optional marked elements and index aliases keeping screen updated.
 *
 * This function takes an array, its size, and optional marked elements and index aliases
 * to draw a visualization of the array on the screen. The marked elements are drawn with
 * different colors and their corresponding index aliases are printed on the screen.
 * The function also initializes the av_view singleton object if it has not been initialized yet.
 * Sets atexit to clean up memory allocated by the library and loops keeping window open and displaying
 * either old history on request or continues execution to next itteration on request from user.
 *
 * @param array A pointer to an array of integers representing the data to be drawn.
 * @param arraySize The size of the array parameter.
 * @param markedIndexes A pointer to an array of integers representing the marked indexes (default is NULL).
 * @param indexAlias A pointer to an array of C-style strings representing the index aliases (default is NULL).
 * @param markedSize The number of marked indexes in the array (default is 0).
 */
static void av_draw(int array[], int arraySize, int markedIndexes[], const char * indexAlias[], int markedSize) {
    av_drawWithSizes(array, arraySize, markedIndexes, indexAlias, markedSize, DEFAULT_KEYBOX_HEIGHT, DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);
}

/**
 * @brief Ends the array visualization and displays the final state of the array.
 *
 * This function takes the final state of the array and its size to draw the last
 * visualization of the array on the screen. It appends the final state of the array
 * to the history and allows the user to navigate through the history using 'A' and 'D' keys.
 *
 * @param array A pointer to an array of integers representing the final state of the data.
 * @param arraySize The size of the array parameter.
 */
static void av_end(int array[], int arraySize) {
    if (av_view->currentSize == av_view->historySize) {
        av_view->historySize += 100;
        av_view->history = (AV_HistoryItem *)realloc(av_view->history, av_view->historySize * sizeof(AV_HistoryItem));
    }
    av_appendHistory(array, arraySize, 0, NULL, NULL);
    //calculating variables that are usful for drawing the array
    int width = av_view->windowWidth/arraySize;
    tigrClear(av_view->screen, BACKGROUNDCOLOR);
    tigrRect(av_view->screen, 0, av_view->windowHeight - av_view->keyboxHeight, (int)av_view->windowWidth, (int)av_view->keyboxHeight, tigrRGB(0xFF, 0xFF, 0xFF));
    for(int i = 0; i < arraySize; i++){
        TPixel c = tigrRGB(0x42, 0xFF, 0xFF);
        tigrFillRect(av_view->screen, width * i, av_view->windowHeight - av_view->keyboxHeight - 10*array[i], width, 10*array[i], c);
    }
    int currentView = av_view->currentSize - 1;
    av_drawArrayUtilityWithDefaults(array, arraySize);
    bool nextItteration = false;
    while(!nextItteration){
        if(tigrKeyDown(av_view->screen, 'D')){
            nextItteration = currentView == av_view->currentSize - 1;
            if(!nextItteration){
                currentView += 1;
                av_drawArrayUtility(av_view->history[currentView].array, av_view->history[currentView].arraySize, av_view->history[currentView].markedSize, av_view->history[currentView].markedIndexes, av_view->history[currentView].indexAliases);
            }
        }
        else if(tigrKeyDown(av_view->screen, 'A') && currentView > 0){
            currentView -= 1;
                av_drawArrayUtility(av_view->history[currentView].array, av_view->history[currentView].arraySize, av_view->history[currentView].markedSize, av_view->history[currentView].markedIndexes, av_view->history[currentView].indexAliases);
        }
        tigrUpdate(av_view->screen);
    }
}

/**
 * @brief Cleanup function to free all allocated memory and resources.
 *
 * This function should be called atexit() so the user does not have to worry about cleanup.
 * It takes care of freeing memory allocated for the AlgoView and its associated resources.
 */
static void av_cleanup() {
    av_serializeAndSaveDefaultFile();
    if (av_view != NULL) {
        // Free the Tigr screen resource
        tigrFree(av_view->screen);

        // Free the memory allocated for the history items
        for (int i = 0; i < av_view->currentSize; i++) {
            free(av_view->history[i].array);

            free(av_view->history[i].indexAliases);
            free(av_view->history[i].markedIndexes);
        }

        // Free the memory allocated for the history array
        free(av_view->history);

        // Free the memory allocated for the AlgoView structure and set the pointer to NULL
        free(av_view);
        av_view = NULL;
    }
}

/**
 * @brief Function to serialize and save history for later viewing
 *
 * This function should be called by the user with a proveded filename
 * @param filename const char* representing the name of the serialize history file to save
 */
static void av_serializeAndSave(const char* filename){
    FILE* file;
    file = fopen(filename, "wb");
    if(file != NULL){
        fwrite(&av_view->historySize, sizeof(int), 1, file);
        fwrite(&av_view->currentSize, sizeof(int), 1, file);

        for (int i = 0; i < av_view->currentSize; i++) {
            AV_HistoryItem *item = &av_view->history[i];

            fwrite(&item->arraySize, sizeof(int), 1, file);
            fwrite(&item->markedSize, sizeof(int), 1, file);

            fwrite(item->array, sizeof(int), item->arraySize, file);
            fwrite(item->markedIndexes, sizeof(int), item->markedSize, file);

            for (int j = 0; j < item->markedSize; j++) {
                int alias_len = strlen(item->indexAliases[j]) + 1;
                fwrite(&alias_len, sizeof(int), 1, file);
                fwrite(item->indexAliases[j], sizeof(char), alias_len, file);
            }

        }
        fclose(file);

    }
    else{
        printf("Issues opening the file for saving serialized history so the file will not be saved.");
    }
    return;
}

/**
 * @brief Function to serialize and save history for later viewing
 *
 * This function should be called at exit so the users history is always saved even when the program
 * crashes ahead of time. This version will assume a filename of serializedFile.dat
 */
static void av_serializeAndSaveDefaultFile(){
    av_serializeAndSave("serializedFile.dat");
}

#endif