#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "algoview.h"
#include "tigr.h"

#define ARRAY_SIZE 50

void swap(int *array, int p, int q){
    int temp = array[p];
    array[p] = array[q];
    array[q] = temp;
}

void sort(int array[], int low, int high){
    //only keep sorting if not done yet obviously
    if(high > low){
        //first pick a pivot
        int pivotIndex = (rand() % (high - low)) + low;

        int indexes[3] = {low, high, pivotIndex};
        std::string tags[3] = {"low", "high", "pivot"};
        av_draw(array, ARRAY_SIZE, indexes, tags, 3);
        std::cout << "In the sort\n";
        int count = -1; //starting at -1 becouse the next step will double count

        for(int i = low; i <= high; i++){
            if(array[pivotIndex] >= array[i]){
                count++;
            }
        }

        //swap the pivot into its correct position
        swap(array, low + count, pivotIndex);
        pivotIndex = low + count;

        //only do the next step if the pivot belonged at an extreeme
        if(pivotIndex != low && pivotIndex != high){
            int i = low;
            int j = high;
            while(i < pivotIndex && j > pivotIndex){
                bool swapCheck = true;
                if(array[i] < array[pivotIndex]){
                    i++;
                    swapCheck = false;
                }
                if(array[j] > array[pivotIndex]){
                    j--;
                    swapCheck = false;
                }
                if(array[i] == array[pivotIndex] && array[pivotIndex] == array[j]){
                    i++;
                }
                if(swapCheck){
                    swap(array, i, j);
                }
            }
        }
        sort(array, low, pivotIndex -1);
        sort(array, pivotIndex + 1, high);
    }
}  


int main(int argc, char* argv[]){
    

    //prep
    int array[ARRAY_SIZE];
    srand(time(nullptr));
    for(int i = 0; i < ARRAY_SIZE; i++){
        array[i] = rand() % 30;
    }

    for(int i = 0; i < ARRAY_SIZE; i++){
        std::cout << array[i] << " | ";
    }
    sort(array, 0, ARRAY_SIZE - 1);
    std::cout << std::endl;

    for(int i = 0; i < ARRAY_SIZE; i++){
        std::cout << array[i] << " | ";
    }
    av_end(array, ARRAY_SIZE);
    return 0;
}