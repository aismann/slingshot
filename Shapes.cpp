#include "Shapes.h"

int getLeftX(Rectangle* rect) {
    return rect->center[0] - rect->width / 2;
}
int getRightX(Rectangle* rect) {
    return rect->center[0] + rect->width / 2;
}
int getTopY(Rectangle* rect) {
    return rect->center[1] - rect->height / 2;
}
int getBottomY(Rectangle* rect) {
    return rect->center[1] + rect->height / 2;
}

int directionOfHit(Circle* circle, Rectangle* rect) {
    //initialize the x and y position of the closePoint vector
    glm::vec2 close_point = circle->center;
    int flag = 0;

    //if the circle's center point is less than the left edge of the rectangle
    if (circle->center[0] < getLeftX(rect)) {
        close_point[0] = getLeftX(rect);
        flag = 1;
    }
    //if circle center is on right side
    else if (circle->center[0] > getRightX(rect)) {
        close_point[0] = getRightX(rect);
        flag = 2;
    }
    //if circle is on top
    if (circle->center[1] < getTopY(rect)) {
        close_point[1] = getTopY(rect);
        flag = 3;
    }
    //bottom edge
    else if (circle->center[1] > getBottomY(rect)) {
        close_point[1] = getBottomY(rect);
        flag = 4;
    }

    //hits edge 
    if (circle->center[0] != close_point[0] && circle->center[1] != close_point[1]) {
        flag = 5;
    }

    glm::vec2 distance = circle->center - close_point;

    if (glm::length(distance) < circle->radius) {
        //make sure that the obj is not inside the platform when the physics step is applied. 
        glm::vec2 penetration_depth = glm::normalize(distance) * (circle->radius - glm::length(distance));
        circle->center += penetration_depth * 2.0f;
        return flag;
    }

    return 0;
}

void debugInfo(Rectangle rectangle,bool coordinates) {
    if (coordinates) {
        printf("[%d,%d], [%d,%d], [%d,%d], [%d,%d] \n", (int)(rectangle.center[0] - rectangle.width / 2), (int)(rectangle.center[1] - rectangle.height / 2),
            (int)(rectangle.center[0] - rectangle.width / 2), (int)(rectangle.center[1] + rectangle.height / 2),
            (int)(rectangle.center[0] + rectangle.width / 2), (int)(rectangle.center[1] - rectangle.height / 2),
            (int)(rectangle.center[0] + rectangle.width / 2), (int)(rectangle.center[1] + rectangle.height / 2)
        );
    }
    else {
        printf("Center = (%d, %d) | Width = %d | Height = %d", (int)(rectangle.center[0]), (int)(rectangle.center[1]), rectangle.width, rectangle.height);

    }

}

void RenderShape(Circle* circle, SDL_Color color) {
    SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a);

    int sides = 20;
    if (sides == 0)
    {
        sides = M_PI * circle->radius;
    }

    float d_a = 2 * M_PI / sides,
        angle = d_a;

    glm::vec2 start, end;
    end.x = circle->radius;
    end.y = 0.0f;
    end = end + circle->center;
    for (int i = 0; i != sides; i++)
    {
        start = end;
        end.x = cos(angle) * circle->radius;
        end.y = sin(angle) * circle->radius;
        end = end + circle->center;
        angle += d_a;
        SDL_RenderDrawLine(Renderer, start[0], start[1], end[0], end[1]);
    }
}

void RenderShape(Rectangle* rectangle, SDL_Color color) {
    SDL_SetRenderDrawColor(Renderer, color.r, color.g, color.b, color.a);
    SDL_Rect temp = { (int)(rectangle->center[0] - rectangle->width / 2) , (int)(rectangle->center[1] - rectangle->height / 2), rectangle->width, rectangle->height };
    SDL_RenderFillRect(Renderer, &temp);
}
