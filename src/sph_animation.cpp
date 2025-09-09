#include "sph_animation.h"

SPHAnimation::SPHAnimation() {
	_particle_system = std::make_shared<ParticleSystem2D>();
}

SPHAnimation::SPHAnimation(const int num_particles) {
	_particle_system = std::make_shared<ParticleSystem2D>();
	resize_particle_system(num_particles);
}

SPHAnimation::~SPHAnimation() {
}

void SPHAnimation::resize_particle_system(size_t n) {
	_particle_system->resize(n);
}

void SPHAnimation::on_update(const float delta) {
	accumulate_forces();
	integrate(delta);
	handle_collisions(delta);
	apply_constraints(delta);
	update_graphics();
}

void SPHAnimation::accumulate_forces() {
	for (size_t i = 0; i < _particle_system->n_particles(); ++i) {
		cato::Vec2& force = _particle_system->get_force(i);
		force.x = 0.0f;
		force.y = -9.81f * _particle_system->mass();
		//_particle_system->get_force(i) = force;
	}
}

void SPHAnimation::integrate(const float delta) {
	// Simple Euler integration
	for (size_t i = 0; i < _particle_system->n_particles(); i++) {
		cato::Vec2& velocity = _particle_system->get_velocity(i);
		cato::Vec2& position = _particle_system->get_position(i);
		cato::Vec2& force = _particle_system->get_force(i);

		velocity += force * delta / _particle_system->mass();
		position += velocity * delta;

		// Reset force for next accumulation
		force.x = 0.0f;
		force.y = 0.0f;
	}
}

void SPHAnimation::handle_collisions(const float delta) {
	// Placeholder for collision handling logic
	CollisionHandler::apply_boundary_collisions(
		_particle_system->get_positions(),
		_particle_system->get_velocities(),
		delta,
		0.0f, 800.0f, 0.0f, 600.0f
	);
}

void SPHAnimation::apply_constraints(const float delta) {
	// Placeholder for constraint application logic
}

void SPHAnimation::update_graphics() {
	for (size_t i = 0; i < _particle_system->n_particles(); ++i) {
		cato::Vec2 pos = _particle_system->get_position(i);
		_particle_system->draw_circle(pos.x, pos.y, _particle_system->radius());
	}
}

