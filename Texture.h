#pragma once
#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <string>
#include "SDL.h"
#include "AssetManager.h"

//renamme to id
typedef struct TextureID {
	SDL_Rect src;
	std::string name;
};

void RenderTextureID(TextureID* tid, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

void setTextureID(TextureID* tid, std::string name);

#endif