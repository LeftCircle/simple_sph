#include "particle_system_solver.h"


template <typename VecType>
ParticleSystemSolver<VecType>::ParticleSystemSolver() {
    _particle_system = std::make_shared<ParticleSystem<VecType>>();
}

template <typename VecType>
ParticleSystemSolver<VecType>::ParticleSystemSolver(size_t n_particles) {
    _particle_system = std::make_shared<ParticleSystem<VecType>>(n_particles);
}

template <typename VecType>
ParticleSystemSolver<VecType>::ParticleSystemSolver(const std::shared_ptr<ParticleSystem<VecType>> particle_system) {
    _particle_system = particle_system;
}

template <typename VecType>
void ParticleSystemSolver<VecType>::set_particle_system(const std::shared_ptr<ParticleSystem<VecType>> particle_system) {
    _particle_system = particle_system;
}

template <typename VecType>
ParticleSystemSolver<VecType>::~ParticleSystemSolver() {}

template <typename VecType>
void ParticleSystemSolver<VecType>::add_interaction_force(const VecType& position, T radius, T strength, T direction) {
    _interactive_force_position = position;
    _interactive_force_radius = radius;
    _interactive_force_strength = strength;
    _interactive_force_direction = direction > 0 ? static_cast<T>(1) : static_cast<T>(-1);
}

template <typename VecType>
void ParticleSystemSolver<VecType>::on_update(double delta) {
    begin_advance_timestep(delta);
    accumulate_forces();
    integrate(delta);
    handle_collisions(delta);
    end_advance_timestep(delta);
}

template <typename VecType>
void ParticleSystemSolver<VecType>::begin_advance_timestep(double time_step_sec) {
    size_t n_particles = _particle_system->n_particles();
    _new_positions.resize(n_particles);
    _new_velocities.resize(n_particles);

    // Clear forces
    auto& forces = _particle_system->get_forces();
    #pragma omp parallel for
    for (size_t i = 0; i < n_particles; ++i) {
        forces[i].x = static_cast<T>(0);
        forces[i].y = static_cast<T>(0);
        if constexpr (std::is_same<VecType, cato::Vec3T<decltype (VecType().x)>>::value) {
			forces[i].z = static_cast<T>(0);
		}
	}
    on_begin_advance_timestep(time_step_sec);
}

template <typename VecType>
void ParticleSystemSolver<VecType>::end_advance_timestep(double time_step_sec) {
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


template <typename VecType>
void ParticleSystemSolver<VecType>::integrate(double time_step_sec) {
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

template<typename VecType>
void ParticleSystemSolver<VecType>::handle_collisions(double time_step_sec) {
	// Placeholder for collision handling logic
	CollisionHandler::apply_boundary_collisions(
		_new_positions,
		_new_velocities,
		cato::Vec2T<T>(0.0, 0.0),
		cato::Vec2T<T>(800.0, 600.0),
		0.6
	);
}

template<typename VecType>
void ParticleSystemSolver<VecType>::apply_constraints(double time_step_sec) {
    // Placeholder for constraint application logic
}

template<typename VecType>
void ParticleSystemSolver<VecType>::accumulate_forces() {
    //Simple gravity force accumulation
    size_t n_particles = _particle_system->n_particles();
    auto& forces = _particle_system->get_forces();
    #pragma omp parallel for
	for (size_t i = 0; i < n_particles; i++) {
		auto& force = _particle_system->get_force(i);
		force.y += _gravity * _particle_system->mass();
	}
    apply_interaction_forces();
}

template<typename VecType>
void ParticleSystemSolver<VecType>::apply_interaction_forces() {
    if (std::abs(_interactive_force_strength) < 0.0001) {
        return;
    }
    size_t n_particles = _particle_system->n_particles();
    auto& forces = _particle_system->get_forces();
    auto& positions = _particle_system->get_positions();
    #pragma omp parallel for
    for (size_t i = 0; i < n_particles; ++i) {
        auto p_to_interaction = _interactive_force_position - positions[i];
        T dist_sq = p_to_interaction.magnitude_squared();
        T radius_sq = _interactive_force_radius * _interactive_force_radius;
        if (dist_sq < radius_sq) {
            T dist = std::sqrt(dist_sq);
            T falloff = 1.0 - (dist_sq / radius_sq);
            auto dir = p_to_interaction.normalized();
            auto interaction_force = dir * (_interactive_force_strength * falloff * _interactive_force_direction);
            forces[i] += interaction_force + forces[i].magnitude() * dir * _interactive_force_direction * 0.5;
        }
    }
}

template <typename VecType>
void ParticleSystemSolver<VecType>::update_graphics() {
	if constexpr (std::is_same<VecType, cato::Vec2T<typename VecType::value_type>>::value){
		for (size_t i = 0; i < _particle_system->n_particles(); ++i) {
			const VecType& pos = _particle_system->get_position(i);
			draw_circle(pos.x, pos.y, _particle_system->radius() / static_cast<T>(10));
		}
	}
	else {
		std::cout << "Graphics update not implemented for 3D particle systems." << std::endl;
	}
}

// explicit type instantiation
template class ParticleSystemSolver<cato::Vec2d>;
template class ParticleSystemSolver<cato::Vec2f>;