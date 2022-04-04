//
// Created by Noah Schonhorn on 4/22/21.
//

#ifndef SMBSDL_UTILS_H
#define SMBSDL_UTILS_H

#include <vector>

#include "SDL.h"

bool init(SDL_Window** window, SDL_Renderer** renderer, int screenWidth, int screenHeight);
void close(SDL_Window** window, SDL_Renderer** renderer);

#endif //SMBSDL_UTILS_H
