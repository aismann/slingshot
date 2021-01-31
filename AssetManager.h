#pragma once

#ifndef _ASSETMANAGER_H
#define _ASSETMANAGER_H

#include <iostream>
#include <map>
#include <SDL.h>
#include "Context.h"

std::map<std::string, SDL_Texture*> GraphicsManager; 

void addTexture(std::string name, std::string filename) {
	GraphicsManager[name] = loadTexture(filename);
}

void removeTexture(std::string name) {
	SDL_Texture* temp = GraphicsManager[name];
	SDL_DestroyTexture(temp);
	temp = NULL;
	GraphicsManager.erase(name);
}
#endif // !_ASSETMANAGER_H

