#include "physics_animation.h"

PhysicsAnimation::PhysicsAnimation() {
}

PhysicsAnimation::~PhysicsAnimation() {
}

void PhysicsAnimation::update(const float delta) {
	on_update(delta);
}

void PhysicsAnimation::on_update(const float delta) {
	// Default implementation does nothing
}

void PhysicsAnimation::accumulate_forces() {
	// Default implementation does nothing
}

void PhysicsAnimation::integrate(const float delta) {
	// Default implementation does nothing
}

void PhysicsAnimation::handle_collisions(const float delta) {
	// Default implementation does nothing
}

void PhysicsAnimation::apply_constraints(const float delta) {
	// Default implementation does nothing
}

void PhysicsAnimation::update_graphics() {
	// Default implementation does nothing
}

