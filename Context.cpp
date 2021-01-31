
#include "Context.h"


const int normalized_tile = 64;
const int SCREEN_WIDTH = normalized_tile * 16;
const int SCREEN_HEIGHT = normalized_tile * 9;
const int FRAME_RATE = 60;
const int FRAME_DELAY = 1000 / FRAME_RATE;


SDL_Window* Window;
SDL_Renderer* Renderer;

bool initialize_framework() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("SDL could not be initialized! Error: %s", SDL_GetError());
		return false;
	}
	if (IMG_Init(IMG_INIT_PNG) < 0) {
		printf("IMG could not be initialized! Error %s\n", IMG_GetError());
		return false;
	}
	if (TTF_Init() < 0) {
		printf("TTF could not be initialized! Error %s\n", TTF_GetError());
		return false;
	}
	Window = SDL_CreateWindow("slingshot", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (Window == NULL) {
		printf("Window could not be initialized! Error: %s\n", SDL_GetError());
		return false;
	}
	Renderer = SDL_CreateRenderer(Window, -1, SDL_RENDERER_ACCELERATED);
	if (Renderer == NULL) {
		printf("Renderer could not be initialized! Error: %s\n", SDL_GetError());
		return false;
	}

	return true;
}

void close_framework() {

	SDL_Quit();
	IMG_Quit();
	TTF_Quit();

	SDL_DestroyWindow(Window);
	SDL_DestroyRenderer(Renderer);

	Window = NULL;
	Renderer = NULL;

}

void renderGridLines() {

	SDL_SetRenderDrawColor(Renderer, 255, 255, 255, 255);
	//horizontal lines
	for (int i = 0; i < 9; i++) {
		SDL_RenderDrawLine(Renderer, 0, i * normalized_tile, SCREEN_WIDTH, i * normalized_tile);
	}

	//vertical lines
	for (int j = 0; j < 16; j++) {
		SDL_RenderDrawLine(Renderer, j * normalized_tile, 0, j * normalized_tile, SCREEN_HEIGHT);
	}
}

SDL_Texture* loadTexture(std::string filename) {
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(filename.c_str());
	if (loadedSurface == NULL)
	{
		printf("Unable to load image %s! SDL_image Error: %s\n", filename.c_str(), IMG_GetError());
	}
	else
	{
		//Create texture from surface pixels
		newTexture = SDL_CreateTextureFromSurface(Renderer, loadedSurface);
		if (newTexture == NULL)
		{
			printf("Unable to create texture from %s! SDL Error: %s\n", filename.c_str(), SDL_GetError());
		}

		//Get rid of old loaded surface
		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}

