#include "sph_system_solver.h"

template <typename VecType>
SPHSystemSolver<VecType>::SPHSystemSolver() : ParticleSystemSolver<VecType>() {
    auto particles = std::make_shared<SPHSystemData<VecType>>();
    this->set_particle_system(particles);
}

template <typename VecType>
SPHSystemSolver<VecType>::SPHSystemSolver(size_t n_particles) : ParticleSystemSolver<VecType>() {
    auto particles = std::make_shared<SPHSystemData<VecType>>(n_particles);
    this->set_particle_system(particles);
}

template <typename VecType>
SPHSystemSolver<VecType>::~SPHSystemSolver() {}


template <typename VecType>
void SPHSystemSolver<VecType>::on_update(double delta){
    ParticleSystemSolver<VecType>::on_update(delta);
}

template <typename VecType>
void SPHSystemSolver<VecType>::on_begin_advance_timestep(double time_step_sec) {
    auto particles = sphSystemData();

    auto& p = particles->get_positions();
    auto& v = particles->get_velocities();
    // Let's build the predicted positions and velocities
    size_t n_particles = particles->n_particles();
    _predicted_positions.resize(n_particles);
    _predicted_velocities.resize(n_particles);
    
    #pragma omp parallel for
    for (size_t i = 0; i < n_particles; ++i) {
        _predicted_velocities[i] = v[i] + time_step_sec * particles->get_force(i) / particles->mass();
        _predicted_positions[i] = p[i] + time_step_sec * _predicted_velocities[i];
    }
        

    const double cell_size = particles->radius() * 2.0;
    auto& boundary_dim = this->_boundary_box.get_dimensions();
    const int resolution_x = static_cast<int>(boundary_dim.x / cell_size) + 1;
    const int resolution_y = static_cast<int>(boundary_dim.y / cell_size) + 1;
	if constexpr (std::is_same<VecType, cato::Vec2T<typename VecType::value_type>>::value){
		particles->build_neighbor_lookup(cato::Vec2i(resolution_x, resolution_y), cell_size, _predicted_positions);
	}
	else {
		const int resolution_z = static_cast<int>(boundary_dim.z / cell_size) + 1;
		particles->build_neighbor_lookup(cato::Vec3i(resolution_x, resolution_y, resolution_z), cell_size, _predicted_positions);
	}
    particles->find_each_neighbor();
    
    particles->update_densities(_predicted_positions);
}

template <typename VecType>
void SPHSystemSolver<VecType>::accumulate_forces() {
    accumulate_pressure_forces();
    accumulate_non_pressure_forces();
}

template <typename VecType>
void SPHSystemSolver<VecType>::accumulate_non_pressure_forces() {
    ParticleSystemSolver<VecType>::accumulate_forces();
    accumulate_viscosity_forces();
}

template <typename VecType>
void SPHSystemSolver<VecType>::accumulate_pressure_forces() {
    auto particles = sphSystemData();
    auto& x = particles->get_positions();
    auto& d = particles->get_densities();
    auto& p = particles->get_pressures();
    auto& f = particles->get_forces();
    compute_pressure();
    accumulate_pressure_forces(x, d, p, f);
}

template <typename VecType>
void SPHSystemSolver<VecType>::accumulate_pressure_forces(
    const std::vector<VecType>& positions,
    const std::vector<T>& densities,
    const std::vector<T>& pressures,
    std::vector<VecType>& forces
) {
    auto particles = sphSystemData();
    size_t n_particles = particles->n_particles();
    const T mass_sq = particles->mass() * particles->mass();
    const Kernel kernel(particles->radius());

    #pragma omp parallel for
    for (size_t i = 0; i < n_particles; ++i) {
        const auto& neighbors = particles->get_neighbors(i);
        for (size_t j : neighbors) {
            VecType rij = positions[i] - positions[j];
            T dist = rij.magnitude();
            if (dist > 0.0001) {
                VecType dir = rij / dist;                
                T pressure_term = (pressures[i] / (densities[i] * densities[i]) +
                                   pressures[j] / (densities[j] * densities[j]));
                VecType pressure_force = mass_sq * pressure_term * kernel.gradient(dist, dir);
                forces[i] += pressure_force;
            }
        }
    }  
}

template <typename VecType>
void SPHSystemSolver<VecType>::accumulate_viscosity_forces() {
    auto particles = sphSystemData();
    auto& x = particles->get_positions();
    auto& v = particles->get_velocities();
    auto& d = particles->get_densities();
    auto& f = particles->get_forces();

    const auto mass_sq = particles->mass() * particles->mass();
    const Kernel kernel(particles->radius());
    size_t n_particles = particles->n_particles();

    #pragma omp parallel for
    for (size_t i = 0; i < n_particles; ++i) {
        const auto& neighbors = particles->get_neighbors(i);
        for (size_t j : neighbors) {
            auto dist = x[i].distance_to(x[j]);
            f[i] += get_viscosity_coefficient() * mass_sq *
                (v[j] - v[i]) / d[j] * kernel.second_derivative(dist); 
        }
    }
}

template <typename VecType>
void SPHSystemSolver<VecType>::compute_pressure(){
    auto particles = sphSystemData();
    auto& d = particles->get_densities();
    auto& p = particles->get_pressures();
    size_t n_particles = particles->n_particles();
    T target_density = particles->get_target_density();
    T sos_sq = static_cast<T>(this->speed_of_sound * this->speed_of_sound);
    T eos_scale = static_cast<T>(sos_sq * target_density) / this->_eos_exponent;

    #pragma omp parallel for
    for (size_t i = 0; i < n_particles; ++i) {
        p[i] = compute_pressure_from_eos(
            d[i],
            target_density,
            eos_scale,
            this->_eos_exponent,
            this->negative_pressure_scale
        );
    }
}

template <typename VecType>
double SPHSystemSolver<VecType>::compute_pressure_from_eos(
    T density,
    T target_desnsity,
    T eos_scale,
    T eos_exponent,
    T negative_pressure_scale
) {
    // -1 is out of the std::power in the code/book, but in the equation??
    // pressure = k / eosExponent * (density / targetDensity - 1) ^ eosExponent
    //T pressure = eos_scale / eos_exponent *
    //    (std::pow((density / target_desnsity), eos_exponent) - 1.0);
    
    T pressure = eos_scale / eos_exponent *
        (std::pow((density / target_desnsity - 1.0), eos_exponent));
    if (pressure < 0) {
        pressure *= negative_pressure_scale;
    }
    return pressure;
}

template <typename VecType>
void SPHSystemSolver<VecType>::on_end_advance_timestep(double time_step_sec) {
    compute_psuedo_viscosity(time_step_sec);
}

template <typename VecType>
void SPHSystemSolver<VecType>::compute_psuedo_viscosity(double time_step_sec) {
    auto particles = sphSystemData();
    size_t numberOfParticles = particles->n_particles();
    auto& x = particles->get_positions();
    auto& v = particles->get_velocities();
    auto& d = particles->get_densities();

    const T mass = particles->mass();
    const Kernel kernel(particles->radius());

    std::vector<VecType> smoothedVelocities(numberOfParticles);

    // TODO -> move to it's own function for building smoothed velocities
    #pragma omp parallel for
    for (size_t i = 0; i < numberOfParticles; ++i) {
        T weightSum = 0.0;
        VecType smoothedVelocity(0);

        const auto& neighbors = particles->get_neighbors(i);
        for (size_t j : neighbors) {
            T dist = x[i].distance_to(x[j]);
            T wj = mass / d[j] * kernel(dist);
            weightSum += wj;
            smoothedVelocity += v[j] * wj;
        }
        T wi = mass / d[i];
        weightSum += wi;
        smoothedVelocity += v[i] * wi;

        if (weightSum > 0.0) {
            smoothedVelocity /= weightSum;
        }

        smoothedVelocities[i] = smoothedVelocity;
    }

    double factor = time_step_sec * _pseudoViscosityCoefficient;
    factor = std::clamp(factor, 0.0, 1.0);

    #pragma omp parallel for
    for (size_t i = 0; i < numberOfParticles; ++i) {
        VecType& velocity = particles->get_velocity(i);
        const VecType& smoothed = smoothedVelocities[i];
        velocity += (smoothed - velocity) * static_cast<T>(factor);
    }
}

template <typename VecType>
void SPHSystemSolver<VecType>::update_graphics() {
    T r = sphSystemData()->radius() / static_cast<T>(10);
    auto particles = sphSystemData();
    if constexpr (std::is_same<VecType, cato::Vec2T<typename VecType::value_type>>::value){
		size_t n_particles = sphSystemData()->n_particles();
        for (size_t i = 0; i < n_particles; i++) {
			VecType pos = sphSystemData()->get_position(i);
			//draw_solid_circle(pos.x, pos.y, sphSystemData()->radius() / static_cast<T>(10));
			//draw_circle(pos.x, pos.y, sphSystemData()->radius());
			auto v = particles->get_velocity(i);
			draw_velocity_circle(
				pos.x, pos.y, r,
				v.x, v.y
			);
		}
	} else {
		std::cout << "Only 2D draw is supported atm" << std::endl;
        // Draw the box!
        this->_boundary_box.draw();
	}
}

template <typename VecType>
void SPHSystemSolver<VecType>::update_graphics(
    GLuint particle_shader,
    GLuint boundary_shader,
    const Mat4f& view_matrix,
    const Mat4f& projection_matrix
) {
    T r = sphSystemData()->radius() / static_cast<T>(10);
    auto particles = sphSystemData();
    if constexpr (std::is_same<VecType, cato::Vec2T<typename VecType::value_type>>::value){
        update_graphics();
    } else {
        // Draw the particles
        glUseProgram(particle_shader);
        // Bind the VAO for the particle model
        particles->bind_vao();
        Mat4f model_matrix = Mat4f::identity();
        GLuint view_loc = glGetUniformLocation(particle_shader, "view");
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view_matrix.m);
        GLuint projection_loc = glGetUniformLocation(particle_shader, "projection");
        glUniformMatrix4fv(projection_loc, 1, GL_FALSE, projection_matrix.m);
        for (size_t i = 0; i < sphSystemData()->n_particles(); i++) {
            VecType pos = sphSystemData()->get_position(i);
            // Update the model matrix for the particle
            GLuint model_loc = glGetUniformLocation(particle_shader, "model");
            model_matrix.set_translation(static_cast<float>(pos.x), static_cast<float>(pos.y), static_cast<float>(pos.z));
            glUniformMatrix4fv(model_loc, 1, GL_FALSE, model_matrix.m);
            
            
            // Draw the particle
            particles->draw_particle_model();

            
            //auto v = particles->get_velocity(i);
           
        }

        // Draw the box!
        glUseProgram(boundary_shader);
        model_matrix = Mat4f::identity();
        GLuint model_loc = glGetUniformLocation(boundary_shader, "model");
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model_matrix.m);
        GLuint view_loc_b = glGetUniformLocation(boundary_shader, "view");
        glUniformMatrix4fv(view_loc_b, 1, GL_FALSE, view_matrix.m);
        GLuint projection_loc_b = glGetUniformLocation(boundary_shader, "projection");
        glUniformMatrix4fv(projection_loc_b, 1, GL_FALSE, projection_matrix.m);
        
        this->_boundary_box.draw();
        glUseProgram(0);
    }
};

// explicit template instantiation
template class SPHSystemSolver<cato::Vec2f>;
template class SPHSystemSolver<cato::Vec2d>;
template class SPHSystemSolver<cato::Vec3f>;
template class SPHSystemSolver<cato::Vec3d>;