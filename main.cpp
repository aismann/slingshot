

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <glm.hpp>
#include <vec2.hpp>

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
SDL_Color green = { 0,255,0,255 };

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
const int FRICTION = 1;



void gravity(SDL_Rect* box) {

    int delta = 10; 

    if ((box->y + box->h) <= SCREEN_HEIGHT) {
        box->y = box->y + delta;
    }


}

glm::vec2 pos = glm::vec2(0.0f, 0.0f);
struct MovableObject {
    int size = 50;
    glm::vec2 pos = glm::vec2(0.0f, 0.0f);
    glm::vec2 velocity = glm::vec2(0.0f, 0.0f);
    glm::vec2 acceleration = glm::vec2(0.0f, 0.0f);


};

MovableObject player;
MovableObject bullet;

void render(MovableObject* obj , SDL_Color color) {

    SDL_Rect temp = { obj->pos[0], obj->pos[1] , obj->size, obj->size };
    SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(gRenderer, &temp);

}

void addJump(MovableObject* player) {

    player->velocity[1] = -50;
}

void addGravity(MovableObject* player) {
    if (abs(player->velocity[1]) <= TERMINAL_SPEED) {
        player->velocity[1] += GRAVITY;
    }
}

void addHorizontalVelocity(MovableObject* player, bool left) {
    int push = 3;
    if (left) {
        //TODO: Fix left right issues with terminal speed 
        if (-player->velocity[0] < TERMINNAL_SPEED_X) {
            player->velocity[0] = player->velocity[0] - push; 
        }
    }
    else {
        if (player->velocity[0] < TERMINNAL_SPEED_X) {
            player->velocity[0] = player->velocity[0] + push;
        }
    }
}

void physics(MovableObject* obj) {
    obj->pos[1] += obj->velocity[1];
    obj->pos[0] += obj->velocity[0];
}

void stopVelocityY(MovableObject* player) {
    player->velocity[1] = 0;
}

void stopVelocityX(MovableObject* player) {
    player->velocity[0] = 0;
}


void addFriction(MovableObject* player) {
    if (grounded && player->velocity[0] != 0) {
        if (player->velocity[0] > FRICTION) {
            player->velocity[0] = player->velocity[0] - FRICTION;
        }
        else if(player->velocity[0] < -FRICTION) {
            player->velocity[0] = player->velocity[0] + FRICTION;
        }
        else {
            player->velocity[0] = 0;
        }
    }
}

void shootBullet(MovableObject* player) {
     
}

const float BULLET_SPEED = 4;

//TODO: fiund more elegant solution for angles and negative values
void spawnBullet(SDL_Point* click_position) {



    glm::vec2 pt_start = player.pos;
    glm::vec2 pt_end = glm::vec2(click_position->x, click_position->y);

    glm::vec2 direction = pt_end - pt_start;
    direction = glm::normalize(direction);

    bullet.pos = player.pos;
    bullet.velocity = direction * BULLET_SPEED;

    printf("Mouse click %d %d \n ", click_position->x, click_position->y);
    printf("Player pos %d %d \n ", player.pos[0], player.pos[1]);
    printf("direction un normalized %f %f\n", direction[0], direction[1]);
    printf("pt_start %.2f %.2f\n", pt_start[0], pt_start[1]);
    printf("pt_end %.2f %.2f\n", pt_end[0], pt_end[1]);
    std::cout << "Bullet shit " << bullet.velocity[0] << " " << bullet.velocity[1] << std::endl;
    

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
        if (input.right) {
            addHorizontalVelocity(&player, false);
        }

        if (!grounded) {
            addGravity(&player);
            if ((player.pos[1] + player.size) >= SCREEN_HEIGHT) {
                player.pos[1] = SCREEN_HEIGHT - player.size;
                stopVelocityY(&player);
                //stopVelocityX(&player);
                grounded = true;
            }
            else {
                grounded = false;
            }
        }
        else {
            addFriction(&player);
        }
        if (input.jump && grounded) {
            addJump(&player);
            grounded = false;
        }
        if (input.click) {
            spawnBullet(&input.mPosition);
        }
        

        physics(&player);
        physics(&bullet);


        SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
        SDL_RenderClear(gRenderer);

        render(&player, red);

        render(&bullet, green);
        
        
        SDL_RenderPresent(gRenderer);

        elapsed_ticks = SDL_GetTicks() - frame_start;

        if (elapsed_ticks  < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - elapsed_ticks);
        }
        
    }

    close();

	return 0;
}