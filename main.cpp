

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
#include "Physics.h"
#include "AssetManager.h"
#include "Texture.h"


const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 960;

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




struct MovableObject {
    Circle hitbox = { glm::vec2(0.0f, 0.0f) , normalized_tile/2 };
    PhysicsComponent physics;
    bool on_ground;
};

MovableObject player;
MovableObject bullet;


/*
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
*/

SDL_Rect platform{ normalized_tile*0, normalized_tile*4, normalized_tile*7, normalized_tile*4 };
Rectangle platf = { glm::vec2(platform.x + platform.w / 2, platform.y + platform.h / 2), platform.w, platform.h};


/*
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
*/

void printRect(SDL_Rect rect) {
    printf("[%d %d %d %d]\n", rect.x, rect.y, rect.w, rect.h);
}
bool gravity_applied = false;
int main(int argc, char* args[]) {

	std::cout << initialize_framework();

    int frame_start;
    int elapsed_ticks;

    player.hitbox.center = glm::vec2(normalized_tile * 0, normalized_tile * 0);

    SDL_Rect camera = { 0,0, SCREEN_WIDTH,SCREEN_HEIGHT };

    std::string dustkid_filename = "C:\\Users\\aliha\\Downloads\\sprites\\player\\dustkid\\idle\\idle0001.png";
    std::string bg_filename = "C:\\Users\\aliha\\Downloads\\30_scrolling\\30_scrolling\\bg.png";
    insertTexture("dustkid", dustkid_filename);
    insertTexture("bg", bg_filename);
    TextureID dustkid;
    setTextureID(&dustkid, "dustkid");
    TextureID background;
    setTextureID(&background, "bg");

    
    printRect(dustkid.src);
    printRect(background.src);

    player.hitbox.center[0] = LEVEL_WIDTH / 2;
    player.hitbox.center[1] = LEVEL_HEIGHT / 2;
    

    while (!input.quit) {

        frame_start = SDL_GetTicks();

        updateInputState(&input);
    
        if (input.left) {
            addMomentum(&player.physics, glm::vec2(-4, 0));
        }
        if (input.right) {
            addMomentum(&player.physics, glm::vec2(4, 0));
        }
        if (!player.on_ground && !gravity_applied) {
            //add gravity
            addForce(&player.physics, glm::vec2(0, 1));
            gravity_applied = true;
        }
        //todo::remove when collision with rectangles gets fleshed out
        //if player hits the ground 
        if (player.hitbox.center[1] + player.hitbox.radius > LEVEL_HEIGHT/2) {
            player.on_ground = true;
            //remove gravity
            removeVerticalForce(&player.physics);
            removeVerticalMomentum(&player.physics);
            gravity_applied = false;
        }
        
        if (input.jump && player.on_ground) {
            addMomentum(&player.physics, glm::vec2(0,-30));
            player.on_ground = false;

        }
        
        addFriction(&player.physics);
        integration(&player.hitbox.center, &player.physics);
        
        //TODO convert dimensions into 2d glm vectors
        camera.x = player.hitbox.center[0] - SCREEN_WIDTH / 2;
        camera.y = player.hitbox.center[1] - SCREEN_HEIGHT / 2;

        //Keep the camera in bounds
        if (camera.x < 0)
        {
            camera.x = 0;
        }
        if (camera.y < 0)
        {
            camera.y = 0;
        }
        if (camera.x > LEVEL_WIDTH - camera.w)
        { 
            camera.x = LEVEL_WIDTH - camera.w;
        }
        if (camera.y > LEVEL_HEIGHT - camera.h)
        {
            camera.y = LEVEL_HEIGHT - camera.h;
        }

        SDL_SetRenderDrawColor(Renderer, 0, 0, 0, 0);
        SDL_RenderClear(Renderer);

        RenderTextureID(&background,0,0, &camera);

        RenderTextureID(&dustkid,player.hitbox.center[0] - camera.x,player.hitbox.center[1] - camera.y);


        //RenderShape(&player.hitbox, red, camera.x , camera.y);
        
        
        SDL_RenderPresent(Renderer);

        elapsed_ticks = SDL_GetTicks() - frame_start;

        if (elapsed_ticks  < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - elapsed_ticks);
        }
        
    }


    //TODO write a function that closes the asset manager
    removeTexture("dustkid");
    removeTexture("bg");

    close_framework();

	return 0;
}