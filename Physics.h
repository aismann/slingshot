#pragma once
#ifndef _PHYSICS_H
#define _PHYSICS_H

#include <iostream>
#include "glm.hpp"
#include "vec2.hpp"
#include "Shapes.h"

const int GRAVITY = 5;
const int TERMINAL_VERTICAL_SPEED = 100;
const int TERMINNAL_HORIZONTAL_SPEED = 20;
const int FRICTION = 2;

typedef struct PhysicsComponent {
	glm::vec2 velocity;
	glm::vec2 acceleration;
};

void integration(glm::vec2 *pos, PhysicsComponent *object);
//void gravity(PhysicsComponent* object);
void addForce(PhysicsComponent* object, glm::vec2 force);
void addFriction(PhysicsComponent* object);
void addMomentum(PhysicsComponent* object, glm::vec2 momentum);
void removeVerticalForce(PhysicsComponent* object);
void removeVerticalMomentum(PhysicsComponent* object);
void removeHorizontalForce(PhysicsComponent* object);
void removeHorizontalMomentum(PhysicsComponent* object);

#endif // !_PHYSICS_H
