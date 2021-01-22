

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 574;

SDL_Window* gWindow;
SDL_Renderer* gRenderer;
SDL_Event event;

typedef struct Controller {
	int up = 0;
	int down = 0;
	int left = 0;
	int right = 0;
	int jump = 0;
	int attack = 0;
	int shield = 0;
	bool quit = false;
    int click = 0;
    int touch = 0;
    SDL_Point mPosition = { 0 , 0 };
};

Controller input; 

bool init();
void updateInputState();

bool init(){
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
	gWindow = SDL_CreateWindow("slingshot", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	if (gWindow == NULL) {
		printf("Window could not be initialized %s\n", SDL_GetError());
		return false;
	}
	gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

	return true;
}
void updateInputState(Controller* input) {
  
 
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            input->quit = true;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_UP:
                input->up += 1;
                break;
            case SDLK_DOWN:
                input->down += 1;
                break;
            case SDLK_LEFT:
                input->left += 1;
                break;
            case SDLK_RIGHT:
                input->right += 1;
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.sym) {
            case SDLK_UP:
                input->up = 0;
                break;
            case SDLK_DOWN:
                input->down = 0;
                break;
            case SDLK_LEFT:
                input->left = 0;
                break;
            case SDLK_RIGHT:
                input->right = 0;
                break;
            default:
                break;
            }
            break;
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&input->mPosition.x, &input->mPosition.y);
            break;
        case SDL_MOUSEBUTTONDOWN:
            input->click = 1;
            break;
        case SDL_MOUSEBUTTONUP:
            input->click = 0;
            break;
        default:
            break;
        }
    }
}
int main(int argc, char* args[]) {

	std::cout << init();

    while (!input.quit) {
        updateInputState(&input);

        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);
        
        SDL_RenderPresent(gRenderer);
    }
	return 0;
}