#include "sph_animation.h"

SPHAnimation::SPHAnimation() {
	_particle_system = std::make_shared<ParticleSystem2D<double>>();
}

SPHAnimation::SPHAnimation(const int num_particles) {
	_particle_system = std::make_shared<ParticleSystem2D<double>>();
	resize_particle_system(num_particles);
}

SPHAnimation::~SPHAnimation() {
}

void SPHAnimation::resize_particle_system(size_t n) {
	_particle_system->resize(n);
}

void SPHAnimation::on_update(double delta) {
	// I don't think we are creating the neighbor lookup structure properly. The grid size should
	// be based on the size of the particles
	_particle_system->build_neighbor_lookup(1, 1, 800);
	_particle_system->find_each_neighbor();
	accumulate_forces();
	integrate(delta);
	handle_collisions(delta);
	apply_constraints(delta);
}

void SPHAnimation::accumulate_forces() {
	for (size_t i = 0; i < _particle_system->n_particles(); ++i) {
		cato::Vec2T<double>& force = _particle_system->get_force(i);
		force.x = 0.0;
		force.y = -9.81 * _particle_system->mass();
	}
}

void SPHAnimation::integrate(double delta) {
	// Simple Euler integration
	for (size_t i = 0; i < _particle_system->n_particles(); i++) {
		cato::Vec2T<double>& velocity = _particle_system->get_velocity(i);
		cato::Vec2T<double>& position = _particle_system->get_position(i);
		cato::Vec2T<double>& force = _particle_system->get_force(i);

		// position += velocity * delta * 0.5;
		// velocity += force * delta / _particle_system->mass();
		// position += velocity * delta * 0.5;


		velocity += force * delta / _particle_system->mass() * 0.1;
		position += velocity * delta;

		// Reset force for next accumulation
		force.x = 0.0;
		force.y = 0.0;
	}
}

void SPHAnimation::handle_collisions(double delta) {
	// Placeholder for collision handling logic
	CollisionHandler::apply_boundary_collisions(
		_particle_system->get_positions(),
		_particle_system->get_velocities(),
		0.0, 800.0, 0.0, 600.0, 0.9
	);
}

void SPHAnimation::apply_constraints(double delta) {
	// Placeholder for constraint application logic
}

void SPHAnimation::update_graphics() {
	for (size_t i = 0; i < _particle_system->n_particles(); ++i) {
		cato::Vec2T<double> pos = _particle_system->get_position(i);
		_particle_system->draw_circle(pos.x, pos.y, _particle_system->radius());
	}
}

