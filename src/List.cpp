//
// Created by Noah Schonhorn on 10/27/21.
//

#include "List.h"
#include "SDL.h"
#include <algorithm>
#include <random>
#include <iostream>
#include <chrono>
#include <thread>

List::List(SDL_Renderer** renderer, int screenWidth, int screenHeight)
{
    this->renderer = *renderer;
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    listSize = 100;
    accesses = 0;
    writes = 0;
    populateList(listSize);
    createEmptyTexture();
}

List::List(SDL_Renderer **renderer, int screenWidth, int screenHeight, int listSize)
{
    this->renderer = *renderer;
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->listSize = listSize;
    accesses = 0;
    writes = 0;
    populateList(this->listSize);
    createEmptyTexture();
}

List::List(SDL_Renderer **renderer, int screenWidth, int screenHeight, int listSize, SDL_FRect* destRect)
{
    this->renderer = *renderer;
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
    this->listSize = listSize;
    accesses = 0;
    writes = 0;
    populateList(this->listSize);
    createEmptyTexture();
    this->destRect = destRect;
}

List::~List()
{
    SDL_DestroyTexture(listTexture);
    listTexture = nullptr;
}

int List::findLargest()
{
    int currentLargest = -1;
    for (int i = 0; i < elementList.size(); ++i)
    {
        if (elementList[i].getValue() > currentLargest)
        {
            currentLargest = elementList[i].getValue();
        }
    }

    return currentLargest;
}

void List::renderColumn(int index, SDL_Color color)
{
    if (SDL_GetRenderTarget(renderer) == nullptr)
    {
        SDL_SetRenderTarget(renderer, listTexture);
    }
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRectF(renderer, &elementList[index].getRenderRect());
    SDL_SetRenderTarget(renderer, nullptr);
}

void List::updateColumnRects()
{
    for (int i = 0; i < elementList.size(); ++i)
    {
        float xPos = (float)i * columnWidth;
        float yPos = (float)elementList[i].getValue() * columnHeight;
        SDL_FRect tempRect = {xPos, (float)screenHeight - yPos, columnWidth, yPos};
        elementList[i].setRenderRect(tempRect);
    }
}

void List::swapColumnRects(int idx1, int idx2)
{
    SDL_FRect temp = elementList[idx1].getRenderRect();
    float xPos1 = (float)idx1 * columnWidth;
    float yPos1 = (float)elementList[idx1].getValue() * columnHeight;
    float xPos2 = (float)idx2 * columnWidth;
    float yPos2 = (float)elementList[idx2].getValue() * columnHeight;

    SDL_FRect firstNewRect = {xPos1, (float)screenHeight - yPos1, columnWidth, yPos1};
    SDL_FRect secondNewRect = {xPos2, (float)screenHeight - yPos2, columnWidth, yPos2};
    elementList[idx1].setRenderRect(firstNewRect);
    elementList[idx2].setRenderRect(secondNewRect);
}

void List::shuffleList()
{
    accesses = 0;
    writes = 0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(std::begin(elementList), std::end(elementList), rd);
    updateColumnRects();
}

void List::populateList(int listSize)
{
    elementList.clear();
    this->listSize = listSize;

    for (int i = 0; i < listSize; ++i)
    {
        elementList.push_back(ListElement(i + 1, {0, 0, 0, 0}));
    }

    columnWidth = (float)screenWidth / listSize;
    columnHeight = (float)screenHeight / findLargest();

    updateColumnRects();
}

void List::populateList()
{
    elementList.clear();

    for (int i = 0; i < listSize; ++i)
    {
        elementList.push_back(ListElement(i + 1, {0, 0, 0, 0}));
    }

    columnWidth = (float)screenWidth / listSize;
    columnHeight = (float)screenHeight / findLargest();

    updateColumnRects();
}

void List::render(int col1, int col2, int col3)
{
    Uint8 r = 0xFF;
    Uint8 g = 0xFF;
    Uint8 b = 0xFF;

    SDL_SetRenderTarget(renderer, listTexture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);

    for (int i = 0; i < elementList.size(); ++i)
    {
        SDL_RenderFillRectF(renderer, &elementList[i].getRenderRect());
    }

    if (col1 >= 0)
    {
        SDL_SetRenderDrawColor(renderer, red.r, red.g, red.b, 0xFF);
        SDL_RenderFillRectF(renderer, &elementList[col1].getRenderRect());
    }
    if (col2 >= 0)
    {
        SDL_SetRenderDrawColor(renderer, green.r, green.g, green.b, 0xFF);
        SDL_RenderFillRectF(renderer, &elementList[col2].getRenderRect());
    }
    if (col3 >= 0)
    {
        SDL_SetRenderDrawColor(renderer, green.r, green.g, green.b, 0xFF);
        SDL_RenderFillRectF(renderer, &elementList[col3].getRenderRect());
    }

    SDL_SetRenderTarget(renderer, nullptr);
    SDL_RenderCopyF(renderer, listTexture, nullptr, nullptr);
}

void List::bubbleSort() {
    auto start = std::chrono::high_resolution_clock::now();
    bool haveSwapped = true;
    int maxIndex = elementList.size();
    while (haveSwapped) {
        haveSwapped = false;
        for (int i = 0; i + 1 < maxIndex; ++i) {
            // Compare items at indices i and i+1 and swap if necessary
            if (elementList[i].getValue() > elementList[i + 1].getValue()) {
                std::swap(elementList[i], elementList[i + 1]);
                //swapColumnRects(i, i + 1);
                // Update haveSwapped
                haveSwapped = true;
            }
            SDL_RenderClear(renderer);
            updateColumnRects();
            render(i, maxIndex);
            SDL_RenderPresent(renderer);
        }
        // Update maxIndex
        --maxIndex;
    }

    auto end = std::chrono::high_resolution_clock::now();
    double finalTime = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "Bubble Sort Time: " << finalTime / 1000 << " sec" << std::endl;
    updateColumnRects();
    for (int i = 0; i < elementList.size(); ++i)
    {
        //SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        renderColumn(i, green);
        SDL_RenderPresent(renderer);
    }

}

void List::mergeSortRec(std::vector<ListElement> &vec, int startIndex, int endIndex) {
    // Recursive base case
    if (endIndex == startIndex) {
        // We are down to a single item. Nothing to split or merge.
        return;
    }

    // Recursive calls
    int centerIndex = (startIndex + endIndex) / 2;
    mergeSortRec(vec, startIndex, centerIndex);
    mergeSortRec(vec, centerIndex + 1, endIndex);

    // Merge
    std::vector<ListElement> temp;
    int leftIndex = startIndex;
    int rightIndex = centerIndex + 1;
    while (leftIndex <= centerIndex && rightIndex <= endIndex) {
        if (vec[leftIndex].getValue() <= vec[rightIndex].getValue()) {
            temp.push_back(ListElement(vec[leftIndex].getValue(),{0,0,0,0}));
            ++leftIndex;
        } else {
            temp.push_back(ListElement(vec[rightIndex].getValue(), {0,0,0,0}));
            ++rightIndex;
        }
    }
    // At this point in the code, one half has been fully copied into temp
    // and the other half has at least one item left to copy. We don't know which half,
    // so we loop through both
    while (leftIndex <= centerIndex) {
        temp.push_back(ListElement(vec[leftIndex].getValue(), {0,0,0,0}));
        ++leftIndex;
    }
    while (rightIndex <= endIndex) {
        temp.push_back(ListElement(vec[rightIndex].getValue(), {0,0,0,0}));
        ++rightIndex;
    }
    // At this point in the code, all of the items have been copied into temp in order
    // Copy everything from temp back into vec
    for (int i = 0; i < temp.size(); ++i) {
        vec[i + startIndex] = temp[i];
        updateColumnRects();
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);
        render(i + startIndex, startIndex, endIndex);
        SDL_RenderPresent(renderer);
    }
}

void List::mergeSort() {
    auto start = std::chrono::high_resolution_clock::now();

    mergeSortRec(elementList, 0, elementList.size() - 1);

    auto end = std::chrono::high_resolution_clock::now();
    double finalTime = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "Merge Sort Time: " << finalTime / 1000 << " sec" << std::endl;
}

void List::selectionSort() {
    auto start = std::chrono::high_resolution_clock::now();

    int swapIndex, i, minIndex;
    int temp;
    for (swapIndex = 0; swapIndex < elementList.size(); ++swapIndex) {
        // Loop through vector starting at swapIndex and keep track of min
        minIndex = swapIndex;
        for (i = swapIndex + 1; i < elementList.size(); ++i) {
            if (elementList[i].getValue() < elementList[minIndex].getValue()) {
                minIndex = i;
            }
            updateColumnRects();
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderClear(renderer);
            render(swapIndex, i, minIndex);
            SDL_RenderPresent(renderer);
        }
        // Swap min value into swapIndex spot in vector
        std::swap(elementList[swapIndex], elementList[minIndex]);
    }

    auto end = std::chrono::high_resolution_clock::now();
    double finalTime = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "Selection Sort Time: " << finalTime / 1000 << " sec" << std::endl;
}

void List::insertionSort() {
    auto start = std::chrono::high_resolution_clock::now();

    int unsortedStartIndex, insertIndex;
    int toBeInserted;
    for (unsortedStartIndex = 1; unsortedStartIndex < elementList.size(); ++unsortedStartIndex) {
        toBeInserted = elementList[unsortedStartIndex].getValue();
        // Loop to shift over the larger elements
        insertIndex = unsortedStartIndex - 1;
        while (insertIndex >= 0 && elementList[insertIndex].getValue() > toBeInserted) {
            elementList[insertIndex + 1] = elementList[insertIndex];
            --insertIndex;
            updateColumnRects();
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderClear(renderer);
            render(unsortedStartIndex, toBeInserted, insertIndex);
            SDL_RenderPresent(renderer);
        }
        // Put toBeInserted back into vec
        elementList[insertIndex + 1].setValue(toBeInserted);
        updateColumnRects();
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);
        render(unsortedStartIndex, toBeInserted, insertIndex);
        SDL_RenderPresent(renderer);
    }

    auto end = std::chrono::high_resolution_clock::now();
    double finalTime = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "Insertion Sort Time: " << finalTime / 1000 << " sec" << std::endl;
}

void List::quickSortUnstable() {
    auto start = std::chrono::high_resolution_clock::now();
    quickSortUnstableRec(elementList, 0, elementList.size() - 1);

    updateColumnRects();
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);
    render();
    SDL_RenderPresent(renderer);
    auto end = std::chrono::high_resolution_clock::now();
    double finalTime = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "QuickSort (Unstable) Time: " << finalTime / 1000 << " sec" << std::endl;
}

void List::quickSortUnstableRec(std::vector<ListElement> &vec, int startIndex, int endIndex) {
    if (endIndex <= startIndex) {
        // There is only one element left. Nothing to do.
        return;
    }
    ListElement partition = vec[startIndex];
    ListElement temp = ListElement(0, {0,0,0,0});
    int i;
    int largerElementIndex = startIndex + 1;
    for (i = startIndex; i <= endIndex; ++i) {
        if (vec[i] < partition) {
            // Swap the element with the one at largerElementIndex to keep smaller elements grouped
            temp = vec[i];
            vec[i] = vec[largerElementIndex];
            vec[largerElementIndex] = temp;
            // Update largerElementIndex
            ++largerElementIndex;
        }
        updateColumnRects();
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);
        render(i, startIndex, endIndex);
        SDL_RenderPresent(renderer);
    }
    vec[startIndex] = vec[largerElementIndex - 1];
    vec[largerElementIndex - 1] = partition;

    // Recursive call for less-than-partition side
    quickSortUnstableRec(vec, startIndex, largerElementIndex - 2);
    // Recursive call for greater-than-or-equal-to-partition side
    quickSortUnstableRec(vec, largerElementIndex, endIndex);
}

int List::leftChild(int i) {
    return 2 * i + 1;
}

void List::percolateDown(std::vector<ListElement> &items, int i, int n, int child, ListElement tmp) {
    for(tmp = items[i]; leftChild(i) < n; i = child) {
        child = leftChild(i);
        // choose the child with the larger value
        if (child != n - 1 && items[child] < items[child + 1]) {
            ++child;
        }
        // if the parent is less than the child, swap them
        if (tmp < items[child]) {
            items[i] = items[child];
        } else {
            // parent is >= both children. nothing more to do.
            break;
        }
        updateColumnRects();
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);
        render(n, i, tmp.getValue());
        SDL_RenderPresent(renderer);
    }
    items[i] = tmp;
}

void List::heapSort() {
    auto start = std::chrono::high_resolution_clock::now();
    int i, j, child;
    ListElement temp = ListElement(0, {0,0,0,0}), tmp = ListElement(0, {0,0,0,0});
    // build the heap (with max value at root)
    for (i = elementList.size() / 2 - 1; i >= 0; --i) {
        percolateDown(elementList, i, elementList.size(), child, tmp);
    }
    // keep deleting the max
    for (j = elementList.size() - 1; j > 0; --j) {
        // swap the maximum out
        temp = elementList[0];
        elementList[0] = elementList[j];
        elementList[j] = temp;

        // make it into a heap again
        percolateDown(elementList, 0, j, child, tmp);
    }
    auto end = std::chrono::high_resolution_clock::now();
    double finalTime = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "Heap Sort Time: " << finalTime / 1000 << " sec" << std::endl;
    updateColumnRects();
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);
    render();
    SDL_RenderPresent(renderer);
}

void List::shellSort()
{
    auto start = std::chrono::high_resolution_clock::now();
    //This is Marcin Ciura's gap sequence.
    //Seems arbitrary, but was experimentally derived as the "best known
    //sequence of increments for shell sort algorithm." https://oeis.org/A102549
    std::vector<int> gapSequence = {701, 301, 132, 57, 23, 10, 4, 1};

    for (int gap : gapSequence)
    {
        for (int i = gap; i < elementList.size(); ++i)
        {
            ListElement temp = elementList[i];

            int j;
            for (j = i; j >= gap && elementList[j - gap] > temp; j -= gap)
            {
                elementList[j] = elementList[j - gap];
                updateColumnRects();
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
                SDL_RenderClear(renderer);
                render(i, j, j - gap);
                SDL_RenderPresent(renderer);
            }
            elementList[j] = temp;
            updateColumnRects();
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderClear(renderer);
            render(i, j, j - gap);
            SDL_RenderPresent(renderer);
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    double finalTime = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "Shell Sort Time: " << finalTime / 1000 << " sec" << std::endl;
}

void List::gnomeSort()
{
    std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();

    int index = 0;
    while (index < elementList.size())
    {
        if (index == 0 || elementList[index] >= elementList[index - 1])
        {
            ++index;
        }
        else
        {
            ListElement temp = elementList[index];
            elementList[index] = elementList[index - 1];
            elementList[index - 1] = temp;
            --index;
        }
        updateColumnRects();
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(renderer);
        render(index);
        SDL_RenderPresent(renderer);
    }

    auto end = std::chrono::high_resolution_clock::now();
    double finalTime = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "Gnome Sort Time: " << finalTime / 1000 << " sec" << std::endl;
}

void List::radixSort() {
    std::chrono::steady_clock::time_point start = std::chrono::high_resolution_clock::now();

    int iteration, i, digit, bucket, item;
    int numDigits = 0;
    int currLargest = findLargest();
    while (currLargest != 0)
    {
        currLargest /= 10;
        numDigits++;
    }
    std::vector<std::vector<ListElement>> buckets(10);
    for (iteration = 0; iteration < numDigits; ++iteration) {
        // Copy everything from vec into buckets
        for (i = 0; i < elementList.size(); ++i) {
            digit = (elementList[i].getValue() / int(pow(10, iteration)) % 10);
            buckets[digit].push_back(elementList[i]);
            updateColumnRects();
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderClear(renderer);
            render(i);
            SDL_RenderPresent(renderer);
        }
        // Copy everything from buckets back into vec
        i = 0;
        for (bucket = 0; bucket < buckets.size(); ++bucket) {
            for (item = 0; item < buckets[bucket].size(); ++item) {
                elementList[i] = buckets[bucket][item];
                ++i;
                updateColumnRects();
                SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
                SDL_RenderClear(renderer);
                render(i);
                SDL_RenderPresent(renderer);
            }
            buckets[bucket].clear();
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    double finalTime = std::chrono::duration<double, std::milli>(end - start).count();
    std::cout << "Radix Sort Time: " << finalTime / 1000 << " sec" << std::endl;
}

bool List::createEmptyTexture()
{
    listTexture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, screenWidth, screenHeight);
    if (listTexture == nullptr)
    {
        printf("Unable to create blank texture! SDL Error:%s\n", SDL_GetError());
    }
    return listTexture != nullptr;
}
