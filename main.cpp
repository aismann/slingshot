

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <glm.hpp>
#include <vec2.hpp>
#include <math.h>

#include "Context.h"
#include "Shapes.h"


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
SDL_Color blue = { 0,0,255,0 };


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

void renderCircle(glm::vec2 center, float radius, SDL_Color color);

void gravity(SDL_Rect* box) {

    int delta = 10; 

    if ((box->y + box->h) <= SCREEN_HEIGHT) {
        box->y = box->y + delta;
    }
}


struct MovableObject {
    struct Circle hitbox = { glm::vec2(0.0f, 0.0f) , normalized_tile/2 };
    int radius = 0.5f * normalized_tile;
    glm::vec2 pos = glm::vec2(0.0f, 0.0f);
    glm::vec2 velocity = glm::vec2(0.0f, 0.0f);
    glm::vec2 acceleration = glm::vec2(0.0f, 0.0f);
};

MovableObject player;
MovableObject bullet;


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

    obj->hitbox.center += obj->velocity;
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

const float BULLET_SPEED = 8;

//TODO: fiund more elegant solution for angles and negative values
void shootBulletTowards(SDL_Point* click_position) {

    glm::vec2 pt_start = player.pos;
    glm::vec2 pt_end = glm::vec2(click_position->x, click_position->y);

    glm::vec2 direction = pt_end - pt_start;
    direction = glm::normalize(direction);

    bullet.pos = player.pos;
    bullet.hitbox.center = player.pos;

    bullet.velocity = direction * BULLET_SPEED;


}



SDL_Rect platform{ normalized_tile*0, normalized_tile*4, normalized_tile*7, normalized_tile*4 };
Rectangle platf = { glm::vec2(platform.x + platform.w / 2, platform.y + platform.h / 2), platform.w, platform.h};



int directionHit(MovableObject* obj, SDL_Rect* platform) {
    //0 is for no hit 1 left, 2right 3 up 4 down 5 is for corner.

    //initialize the x and y position of the closePoint vector
    glm::vec2 close_point = obj->pos;
    int flag = 0;
    
    //if the circle's center point is less than the left edge of the rectangle
    if (obj->pos[0] < platform->x) {
        close_point[0] = platform->x;
        flag = 1;
    }
    //if circle center is on right side
    else if (obj->pos[0] > platform->x + platform->w) {
        close_point[0] = platform->x + platform->w;
        flag = 2;
    }
    //if circle is on top
    if (obj->pos[1] < platform->y) {
        close_point[1] = platform->y;
        flag = 3;
    }
    //bottom edge
    else if (obj->pos[1] > platform->y + platform->h) {
        close_point[1] = platform->y + platform->h;
        flag = 4;
    }

    //hits edge 
    if (obj->pos[0] != close_point[0] && obj->pos[1] != close_point[1]) {
        flag = 5;
    }
    
    
    glm::vec2 distance = obj->pos - close_point;
    
    if (glm::length(distance) < obj->radius) {
        //make sure that the obj is not inside the platform when the physics step is applied. 
        glm::vec2 penetration_depth = glm::normalize(distance) * (obj->radius - glm::length(distance));
        obj->pos += penetration_depth*2.0f;
        return flag;
    }

    return 0;
}

void recoil(MovableObject* obj, int dirofhit) {

    //if hit from left or right
    if (dirofhit == 1 || dirofhit == 2) {
        obj->velocity[0] = -obj->velocity[0];
    }
    if (dirofhit == 3 || dirofhit == 4) {
        obj->velocity[1] = -obj->velocity[1];
    }
    if (dirofhit == 5) {
        obj->velocity = -1.0f * obj->velocity;
    }
}

int main(int argc, char* args[]) {

	std::cout << initialize_framework();

    int frame_start;
    int elapsed_ticks;

    debugInfo(platf, true);

    

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
            if ((player.hitbox.center[1] + player.radius) >= SCREEN_HEIGHT) {
                player.hitbox.center[1] = SCREEN_HEIGHT - player.radius;
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
            shootBulletTowards(&input.mPosition);
        }

        int dir2 = directionOfHit(&bullet.hitbox, &platf);
        
        if (dir2) {
            recoil(&bullet, dir2);
        }
        physics(&player);
        physics(&bullet);


        SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 0);
        SDL_RenderClear(Renderer);

        renderGridLines();

        RenderShape(&player.hitbox, red);
        RenderShape(&bullet.hitbox, green);
        RenderShape(&platf, blue);

        SDL_RenderPresent(Renderer);

        elapsed_ticks = SDL_GetTicks() - frame_start;

        if (elapsed_ticks  < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - elapsed_ticks);
        }
        
    }

    close_framework();

	return 0;
}