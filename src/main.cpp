#include <iostream>
#include <thread>
#include <cstdio>
#include "Utils.h"
#include "List.h"

int main()
{
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;

    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 800;

    if (!init(&window, &renderer, SCREEN_WIDTH, SCREEN_HEIGHT))
    {
        printf("Failed to intialize!\n");
    }
    else
    {
        //Main quit flag
        bool quit = false;
        bool useOpenGL = true;

        SDL_SetWindowResizable(window, SDL_TRUE);

        //Renderer colors
        Uint8 r = 0x00;
        Uint8 g = 0x00;
        Uint8 b = 0x00;

        int listSize = 500;

        //Event handler
        SDL_Event e;

        List sortList(&renderer, SCREEN_WIDTH, SCREEN_HEIGHT, listSize);
        sortList.shuffleList();

        std::cout << "Controls:" << std::endl;
        std::cout << "Q/Esc: Quit" << std::endl;
        std::cout << "Arrow Keys: Adjust List Size" << std::endl;
        std::cout << "Space: Re-shuffle List" << std::endl;
        std::cout << "Ctrl-S: Toggle Between Metal and OpenGL backends" << std::endl;
        std::cout << "1:      2:         3:         4:     5:    6:         7:     8:         9:" << std::endl;
        std::cout << "Bubble, Insertion, Selection, Merge, Heap, Quicksort, Shell, Gnome, and Radix respectively." << std::endl;

        while (!quit)
        {
            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                }
                else if (e.type == SDL_KEYDOWN)
                {
                    switch (e.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                        case SDLK_q:
                            quit = true;
                            break;
                        case SDLK_UP:
                            listSize++;
                            sortList.populateList(listSize);
                            std::cout << listSize << std::endl;
                            break;
                        case SDLK_RIGHT:
                            listSize += 5;
                            sortList.populateList(listSize);
                            std::cout << listSize << std::endl;
                            break;
                        case SDLK_DOWN:
                            listSize--;
                            sortList.populateList(listSize);
                            std::cout << listSize << std::endl;
                            break;
                        case SDLK_LEFT:
                            listSize -= 5;
                            sortList.populateList(listSize);
                            std::cout << listSize << std::endl;
                            break;
                        case SDLK_1:
                            sortList.bubbleSort();
                            break;
                        case SDLK_2:
                            sortList.insertionSort();
                            break;
                        case SDLK_3:
                            sortList.selectionSort();
                            break;
                        case SDLK_4:
                            sortList.mergeSort();
                            break;
                        case SDLK_5:
                            sortList.heapSort();
                            break;
                        case SDLK_6:
                            sortList.quickSortUnstable();
                            break;
                        case SDLK_7:
                            sortList.shellSort();
                            break;
                        case SDLK_8:
                            sortList.gnomeSort();
                            break;
                        case SDLK_9:
                            sortList.radixSort();
                            break;
                        case SDLK_SPACE:
                            sortList.shuffleList();
                            break;
                        case SDLK_s:
                            if (e.key.keysym.mod & KMOD_LCTRL || e.key.keysym.mod & KMOD_RCTRL)
                            {
                                useOpenGL = !useOpenGL;
                                if (useOpenGL)
                                {
                                    if (!SDL_SetHint(SDL_HINT_RENDER_DRIVER, "opengl"))
                                    {
                                        printf("Warning: OpenGL not being used!");
                                    }
                                }
                                else
                                {
                                    if (!SDL_SetHint(SDL_HINT_RENDER_DRIVER, "metal"))
                                    {
                                        printf("Warning: Metal not being used!");
                                    }
                                }
                            }
                            break;
                    }
                }
            }
            SDL_SetRenderDrawColor(renderer, r, g, b, 0xFF);
            SDL_RenderClear(renderer);

            sortList.render();

            SDL_RenderPresent(renderer);
        }
    }

    close(&window, &renderer);

    return 0;
}
