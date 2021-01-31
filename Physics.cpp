#include "Physics.h"

void integration(glm::vec2 *pos, PhysicsComponent* object) {
	object->velocity += object->acceleration;
	*pos = *pos + object->velocity;
}
//jumps, gravity, horizontal movement, etc
void addForce(PhysicsComponent *object, glm::vec2 force) {
	object->acceleration += force;
}

void addMomentum(PhysicsComponent* object, glm::vec2 momentum) {
    object->velocity += momentum;

    if (std::abs(object->velocity[0]) >= TERMINNAL_HORIZONTAL_SPEED) {
        if (object->velocity[0] < 0) {
            object->velocity[0] = -TERMINNAL_HORIZONTAL_SPEED;
        }
        else {
            object->velocity[0] = TERMINNAL_HORIZONTAL_SPEED;
        }
    }
    if (std::abs(object->velocity[1]) >= TERMINAL_VERTICAL_SPEED) {
        if (object->velocity[1] < 0) {
            object->velocity[1] = -TERMINAL_VERTICAL_SPEED;
        }
        else {
            object->velocity[1] = TERMINAL_VERTICAL_SPEED;
        }
    }
}
//factor is how much the 
void addFriction(PhysicsComponent* object) {
    if (object->velocity[0] != 0) {
        if (object->velocity[0] > FRICTION) {
            object->velocity[0] = object->velocity[0] - FRICTION;
        }
        else if (object->velocity[0] < -FRICTION) {
            object->velocity[0] = object->velocity[0] + FRICTION;
        }
        else {
            object->velocity[0] = 0;
        }
    }
}

void removeVerticalMomentum(PhysicsComponent* object) {
    object->velocity[1] = 0;
}

void removeVerticalForce(PhysicsComponent* object) {
    object->acceleration[1] = 0;
}

void removeHorizontalMomentum(PhysicsComponent* object) {
    object->velocity[0] = 0;
}

void removeHorizontalForce(PhysicsComponent* object) {
    object->acceleration[0] = 0;
}

