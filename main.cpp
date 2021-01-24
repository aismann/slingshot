

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 574;
const int FRAME_RATE = 60;
const int FRAME_DELAY = 1000 / FRAME_RATE;


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

void close() {

    SDL_Quit();
    IMG_Quit();
    TTF_Quit();

    SDL_DestroyWindow(gWindow);
    SDL_DestroyRenderer(gRenderer);

    gWindow = NULL;
    gRenderer = NULL;

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
                input->up = 1;
                break;
            case SDLK_DOWN:
                input->down = 1;
                break;
            case SDLK_LEFT:
                input->left = 1;
                break;
            case SDLK_RIGHT:
                input->right = 1;
                break;
            case SDLK_a:
                input->jump = 1;
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
            case SDLK_a:
                input->jump = 0;
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


SDL_Color red = { 255,0,0,255 };

void movePlayer(Controller* input, SDL_Rect* box) {

    int horizontal_speed = 5;
    box->x = box->x + input->right*horizontal_speed;
    box->x = box->x - input->left*horizontal_speed;
    box->y = box->y + input->down;
    box->y = box->y - input->up;

    

}

const int GRAVITY = 5;
const int TERMINAL_SPEED = 100;
const int TERMINNAL_SPEED_X = 20;
bool grounded = false;


void gravity(SDL_Rect* box) {

    int delta = 10; 

    if ((box->y + box->h) <= SCREEN_HEIGHT) {
        box->y = box->y + delta;
    }


}

struct MovableObject {
    SDL_Rect box = { 20,20,20,20 };
    int velocity_x = 0;
    int velocity_y = 0; 
    int acceleration_x = 0;
    int acceleration_y = 0;

};

MovableObject player;


void addJump(MovableObject* player) {

    player->velocity_y = -50;
}

void addGravity(MovableObject* player) {
    if (abs(player->velocity_y) <= TERMINAL_SPEED) {
        player->velocity_y += GRAVITY;
    }
}

void addHorizontalVelocity(MovableObject* player, bool left) {
    int push = 3;
    if (left) {
        //TODO: Fix left right issues with terminal speed 
        if (-player->velocity_x < TERMINNAL_SPEED_X) {
            player->velocity_x = player->velocity_x - push; 
        }
    }
    else {
        if (player->velocity_x < TERMINNAL_SPEED_X) {
            player->velocity_x = player->velocity_x + push;
        }
    }
}

void physics(MovableObject* player) {
    player->box.y += player->velocity_y;
    player->box.x += player->velocity_x;

    printf("%d , %d", player->velocity_x, player->velocity_y);
}

void stopVelocityY(MovableObject* player) {
    player->velocity_y = 0;
}

void stopVelocityX(MovableObject* player) {
    player->velocity_x = 0;
}



int main(int argc, char* args[]) {

	std::cout << init();

    int frame_start;
    int elapsed_ticks;

    while (!input.quit) {

        frame_start = SDL_GetTicks();

        updateInputState(&input);


        if (input.left) {
            addHorizontalVelocity(&player, true);
        }
        else if (input.right) {
            addHorizontalVelocity(&player, false);
        }

        if (!grounded) {
            addGravity(&player);
            if ((player.box.y + player.box.h) >= SCREEN_HEIGHT) {
                player.box.y = SCREEN_HEIGHT - player.box.h;
                stopVelocityY(&player);
                stopVelocityX(&player);
                grounded = true;
            }
            else {
                grounded = false;
            }
        }
        if (input.jump && grounded) {
            addJump(&player);
            grounded = false;
        }
        

        physics(&player);


        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);

        SDL_SetRenderDrawColor(gRenderer, red.r, red.g, red.b, red.a);
        SDL_RenderFillRect(gRenderer, &player.box);
        
        
        SDL_RenderPresent(gRenderer);

        elapsed_ticks = SDL_GetTicks() - frame_start;

        if (elapsed_ticks  < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - elapsed_ticks);
        }
        
    }

    close();

	return 0;
}