#include "sph_animation.h"

SPHAnimation::SPHAnimation() {
	_particle_system = std::make_shared<ParticleSystem2D>();
}

SPHAnimation::SPHAnimation(const int num_particles) {
	_particle_system = std::make_shared<ParticleSystem2D>();
	_particle_system->resize(num_particles);
}

SPHAnimation::~SPHAnimation() {
}

void SPHAnimation::on_update(const float delta) {
	accumulate_forces();
	integrate(delta);
	handle_collisions();
	apply_constraints();
	update_graphics();
}

void SPHAnimation::accumulate_forces() {
	// Placeholder for force accumulation logic
}

void SPHAnimation::integrate(const float delta) {
	// Placeholder for integration logic (e.g., Euler, Verlet)
}

void SPHAnimation::handle_collisions() {
	// Placeholder for collision handling logic
}

void SPHAnimation::apply_constraints() {
	// Placeholder for constraint application logic
}

void SPHAnimation::update_graphics() {
	for (size_t i = 0; i < _particle_system->n_particles(); ++i) {
		std::array<float, 2> pos = _particle_system->get_position(i);
		_particle_system->draw_circle(pos[0], pos[1], _particle_system->radius());
	}
}
