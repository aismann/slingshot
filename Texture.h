#pragma once
#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <string>
#include "SDL.h"
#include "AssetManager.h"

typedef struct Texture {
	SDL_Rect src;
	std::string name;
};

void Render(Texture* texture);

#endif