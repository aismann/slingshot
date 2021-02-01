#pragma once
#ifndef _CONTEXT_H
#define _CONTEXT_H

#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"


extern const int normalized_tile;
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern const int FRAME_RATE;
extern const int FRAME_DELAY;

extern SDL_Renderer* Renderer;
extern SDL_Window* Window;

void renderGridLines();

SDL_Texture* loadTexture(std::string filename);
void closeTexture(SDL_Texture* texture);

//initialize sdl packages and window/renderer
bool initialize_framework();
void close_framework();
#endif // _CONTEXT_HEADER
