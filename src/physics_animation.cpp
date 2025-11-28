#include "physics_animation.h"

PhysicsAnimation::PhysicsAnimation() {
}

PhysicsAnimation::~PhysicsAnimation() {
}

void PhysicsAnimation::update(double delta) {
	on_update(delta);
}

void PhysicsAnimation::on_update(double delta) {
	// Default implementation does nothing
}

void PhysicsAnimation::accumulate_forces() {
	// Default implementation does nothing
}

void PhysicsAnimation::integrate(double delta) {
	// Default implementation does nothing
}

void PhysicsAnimation::handle_collisions(double delta) {
	// Default implementation does nothing
}

void PhysicsAnimation::apply_constraints(double delta) {
	// Default implementation does nothing
}

void PhysicsAnimation::update_graphics() {
	// Default implementation does nothing
}

