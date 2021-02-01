#pragma once

#ifndef _ASSETMANAGER_H
#define _ASSETMANAGER_H

#include <iostream>
#include <map>
#include <SDL.h>
#include "Context.h"

extern std::map<std::string, SDL_Texture*> GraphicsManager; 

void insertTexture(std::string name, std::string filepath);

SDL_Texture* queryTexture(std::string name);

void removeTexture(std::string name);


#endif // !_ASSETMANAGER_H

