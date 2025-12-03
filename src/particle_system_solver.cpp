#include "particle_system_solver.h"


template <typename T>
ParticleSystemSolver2D<T>::ParticleSystemSolver2D() {
    _particle_system = std::make_shared<ParticleSystem2D<T>>();
}

template <typename T>
ParticleSystemSolver2D<T>::ParticleSystemSolver2D(size_t n_particles) {
    _particle_system = std::make_shared<ParticleSystem2D<T>>(n_particles);
}

template <typename T>
ParticleSystemSolver2D<T>::ParticleSystemSolver2D(const std::shared_ptr<ParticleSystem2D<T>> particle_system) {
    _particle_system = particle_system;
}

template <typename T>
void ParticleSystemSolver2D<T>::set_particle_system(const std::shared_ptr<ParticleSystem2D<T>> particle_system) {
    _particle_system = particle_system;
}

template <typename T>
ParticleSystemSolver2D<T>::~ParticleSystemSolver2D() {}

template <typename T>
void ParticleSystemSolver2D<T>::add_interaction_force(const cato::Vec2T<T>& position, T radius, T strength, T direction) {
    _interactive_force_position = position;
    _interactive_force_radius = radius;
    _interactive_force_strength = strength;
    _interactive_force_direction = direction > 0 ? static_cast<T>(1) : static_cast<T>(-1);
}

template <typename T>
void ParticleSystemSolver2D<T>::on_update(double delta) {
    begin_advance_timestep(delta);
    accumulate_forces();
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
        new_pos = (positions[i] + time_step_sec * new_vel);
    }
}

template<typename T>
void ParticleSystemSolver2D<T>::handle_collisions(double time_step_sec) {
	// Placeholder for collision handling logic
	CollisionHandler::apply_boundary_collisions(
		_new_positions,
		_new_velocities,
		0.0, 800.0, 0.0, 600.0, 0.6
	);
}

template<typename T>
void ParticleSystemSolver2D<T>::apply_constraints(double time_step_sec) {
    // Placeholder for constraint application logic
}

template<typename T>
void ParticleSystemSolver2D<T>::accumulate_forces() {
    //Simple gravity force accumulation
    size_t n_particles = _particle_system->n_particles();
    auto& forces = _particle_system->get_forces();
    #pragma omp parallel for
	for (size_t i = 0; i < n_particles; i++) {
		cato::Vec2T<T>& force = _particle_system->get_force(i);
		force.y += _gravity * _particle_system->mass();
	}
    apply_interaction_forces();
}

template<typename T>
void ParticleSystemSolver2D<T>::apply_interaction_forces() {
    if (std::abs(_interactive_force_strength) < 0.0001) {
        return;
    }
    std::cout << "Interaction force at (" 
            << _interactive_force_position.x << ", " 
            << _interactive_force_position.y << ")"
            << " with radius " << _interactive_force_radius
            << " and strength " << _interactive_force_strength << "\n";
    size_t n_particles = _particle_system->n_particles();
    auto& forces = _particle_system->get_forces();
    auto& positions = _particle_system->get_positions();
    #pragma omp parallel for
    for (size_t i = 0; i < n_particles; ++i) {
        cato::Vec2T<T> p_to_interaction = _interactive_force_position - positions[i];
        T dist_sq = p_to_interaction.magnitude_squared();
        T radius_sq = _interactive_force_radius * _interactive_force_radius;
        if (dist_sq < radius_sq) {
            T dist = std::sqrt(dist_sq);
            T falloff = 1.0 - (dist_sq / radius_sq);
            cato::Vec2T<T> dir = p_to_interaction.normalized();
            cato::Vec2T<T> interaction_force = dir * (_interactive_force_strength * falloff * _interactive_force_direction);
            forces[i] += interaction_force + forces[i].magnitude() * dir * _interactive_force_direction * 0.5;
        }
    }
}

template <typename T>
void ParticleSystemSolver2D<T>::update_graphics() {
	for (size_t i = 0; i < _particle_system->n_particles(); ++i) {
		cato::Vec2T<T> pos = _particle_system->get_position(i);
		draw_circle(pos.x, pos.y, _particle_system->radius() / static_cast<T>(10));
	}
}

// explicit type instantiation
template class ParticleSystemSolver2D<double>;
template class ParticleSystemSolver2D<float>;