#pragma once
#ifndef _PHYSICS_H
#define _PHYSICS_H

#include <iostream>
#include "glm.hpp"
#include "vec2.hpp"
#include "Shapes.h"

typedef struct PhysicsComponent {
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec2 acceleration;
	bool on_ground; 
};

void integration(PhysicsComponent* object);
void gravity(PhysicsComponent* object);
void addVelocity(PhysicsComponent* object, glm::vec2 push);

#endif // !_PHYSICS_H
