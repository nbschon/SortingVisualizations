//
// Created by Noah Schonhorn on 10/27/21.
//

#ifndef SORTINGVISUALIZATION_LIST_H
#define SORTINGVISUALIZATION_LIST_H

#include <vector>
#include "SDL.h"
#include "ListElement.h"

class List
{
private:
    SDL_Renderer* renderer;
    SDL_Texture* listTexture;
    std::vector<ListElement> elementList;

    SDL_FRect* destRect;

    SDL_Color red = {0xFF, 0x00, 0x00, 0xFF};
    SDL_Color blue = {0x00, 0x00, 0xFF, 0xFF};
    SDL_Color green = {0x00, 0xFF, 0x00, 0xFF};
    SDL_Color white = {0xFF, 0xFF, 0xFF, 0xFF};
    SDL_Color black = {0x00, 0x00, 0x00, 0xFF};

    int listSize;
    int accesses;
    int writes;

    int screenWidth;
    int screenHeight;

    float columnWidth = 0.0f;
    float columnHeight = 0.0f;

    void mergeSortRec(std::vector<ListElement> &vec, int startIndex, int endIndex);
    void quickSortUnstableRec(std::vector<ListElement> &vec, int startIndex, int endIndex);
    int leftChild(int i);
    void percolateDown(std::vector<ListElement> &items, int i, int n, int child, ListElement tmp);

    int findLargest();

    void renderColumn(int index, SDL_Color color);
    void updateColumnRects();
    void swapColumnRects(int idx1, int idx2);

public:
    List(SDL_Renderer** renderer, int screenWidth, int screenHeight);
    List(SDL_Renderer** renderer, int screenWidth, int screenHeight, int listSize);
    List(SDL_Renderer** renderer, int screenWidth, int screenHeight, int listSize, SDL_FRect* destRect);
    ~List();
    void shuffleList();
    void populateList();
    void populateList(int listSize);
    void render(int col1 = -1, int col2 = -1, int col3 = -1);
    bool createEmptyTexture();

    void bubbleSort();
    void mergeSort();
    void selectionSort();
    void insertionSort();
    void quickSortUnstable();
    void heapSort();
    void shellSort();
    void gnomeSort();
    void radixSort();
};


#endif //SORTINGVISUALIZATION_LIST_H
