#include "AssetManager.h"


std::map<std::string, SDL_Texture*> GraphicsManager;

void insertTexture(std::string name, std::string filename) {
	SDL_Texture* query = loadTexture(filename);
	if (query == NULL) {
		printf("%s : FAILED TO LOAD!\n%s : FILEPATH\n" , name.c_str(), filename.c_str());
	}
	else {
		GraphicsManager[name] = loadTexture(filename);
	}
}

void removeTexture(std::string name) {
	closeTexture(GraphicsManager[name]);
	GraphicsManager.erase(name);
}

SDL_Texture* queryTexture(std::string name) {
	SDL_Texture* query = GraphicsManager[name];
	return query;
}


