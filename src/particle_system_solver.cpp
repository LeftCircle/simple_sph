#include "particle_system_solver.h"


template <typename T>
ParticleSystemSolver2D<T>::ParticleSystemSolver2D() {
    _particle_system = std::make_shared<ParticleSystem2D<T>>();
}

template <typename T>
ParticleSystemSolver2D<T>::~ParticleSystemSolver2D() {}

template <typename T>
void ParticleSystemSolver2D<T>::on_update(double delta) {
    begin_advance_timestep(delta);
    accumulate_forces(delta);
    integrate(delta);
    handle_collisions(delta);
    end_advance_timestep(delta);
}

template <typename T>
void ParticleSystemSolver2D<T>::begin_advance_timestep(double time_step_sec) {
    size_t n_particles = _particle_system->n_particles();
    _new_positions.resize(n_particles);
    _new_velocities.resize(n_particles);

    // Clear forces
    auto& forces = _particle_system->get_forces();
    #pragma omp parallel for
    for (size_t i = 0; i < n_particles; ++i) {
        forces[i].x = static_cast<T>(0);
        forces[i].y = static_cast<T>(0);
    }

    on_begin_advance_timestep(time_step_sec);
}

template <typename T>
void ParticleSystemSolver2D<T>::end_advance_timestep(double time_step_sec) {
    // Update particle system to new state
    size_t n_particles = _particle_system->n_particles();
    auto& positions = _particle_system->get_positions();
    auto& velocities = _particle_system->get_velocities();
    #pragma omp parallel for
    for (size_t i = 0; i < n_particles; ++i) {
        positions[i] = _new_positions[i];
        velocities[i] = _new_velocities[i];
    }
    on_end_advance_timestep(time_step_sec);
}


template <typename T>
void ParticleSystemSolver2D<T>::integrate(double time_step_sec) {
    size_t n_particles = _particle_system->n_particles();
    auto& positions = _particle_system->get_positions();
    auto& velocities = _particle_system->get_velocities();
    auto& forces = _particle_system->get_forces();
    const T mass = _particle_system->mass();

    #pragma omp parallel for
    for (size_t i = 0; i < n_particles; ++i) {
        // Simple explicit Euler integration
        cato::Vec2T<T>& new_vel = _new_velocities[i];
        new_vel = velocities[i] + time_step_sec * forces[i] / mass;

        auto& new_pos = _new_positions[i];
        new_pos = positions[i] + time_step_sec * new_vel;

    }
}

template<typename T>
void ParticleSystemSolver2D<T>::handle_collisions(double time_step_sec) {
	// Placeholder for collision handling logic
	CollisionHandler::apply_boundary_collisions(
		_particle_system->get_positions(),
		_particle_system->get_velocities(),
		0.0, 800.0, 0.0, 600.0, 0.9
	);
}


// explicit type instantiation
template class ParticleSystemSolver2D<double>;
template class ParticleSystemSolver2D<float>;