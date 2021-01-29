#include "Physics.h"

void integration(struct PhysicsComponent* object) {
	object->velocity += object->acceleration;
	object->position += object->velocity;
}
