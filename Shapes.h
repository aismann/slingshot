#pragma once
#ifndef _SHAPES_H
#define _SHAPES_H

#include "glm.hpp"
#include "vec2.hpp"
#include <stdio.h>

typedef struct Circle;
typedef struct Rectangle;
typedef struct Point;

typedef struct Point {
	glm::vec2 Pos;
};

typedef struct Circle {
	glm::vec2 center;
	int radius;
};

typedef struct Rectangle {
	glm::vec2 center;
	int width;
	int height;
};

int getLeftX(Rectangle* rect);
int getRightX(Rectangle* rect);
int getTopY(Rectangle* rect);
int getBottomY(Rectangle* rect);

bool collisionCircleAndRectangle(Circle *circle, Rectangle *rect);
bool collisionRectangleAndRectangle(Rectangle *a, Rectangle *b);
bool collisionPointInShape(Point *p, Circle *circle);
bool collisionPointInShape(Point *p, Rectangle *rectangle);
int directionOfHit(Circle *circle, Rectangle *rect);

void debugInfo(Rectangle rectangle,bool coordinates = false);
void debugInfo(Circle circle);
void debugInfo(Point point);

#endif // !_SHAPES_H
