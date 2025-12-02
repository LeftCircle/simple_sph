#include "sph_system_solver.h"

template <typename T>
SPHSystemSolver2T<T>::SPHSystemSolver2T() : ParticleSystemSolver2D<T>() {
    auto particles = std::make_shared<SPHSystemData2<T>>();
    this->set_particle_system(particles);
}

template <typename T>
SPHSystemSolver2T<T>::SPHSystemSolver2T(size_t n_particles) : ParticleSystemSolver2D<T>() {
    auto particles = std::make_shared<SPHSystemData2<T>>(n_particles);
    this->set_particle_system(particles);
}

template <typename T>
SPHSystemSolver2T<T>::~SPHSystemSolver2T() {}


template <typename T>
void SPHSystemSolver2T<T>::on_update(double delta){
    ParticleSystemSolver2D<T>::on_update(delta);
    // std::cout << "Particle 0 pos = (" 
    //           << this->_particle_system->get_position(0).x << ", " 
    //           << this->_particle_system->get_position(0).y << ")\n";
}

template <typename T>
void SPHSystemSolver2T<T>::on_begin_advance_timestep(double time_step_sec) {
    auto particles = sphSystemData();
    const double cell_size = particles->radius() * 2.0;
    const int resolution_x = static_cast<int>(800.0 / cell_size) + 1;
    const int resolution_y = static_cast<int>(600.0 / cell_size) + 1;
    particles->build_neighbor_lookup(resolution_x, resolution_y, cell_size);
    particles->find_each_neighbor();
    // Let's print out the particle neighbor list
    // for (size_t i = 0; i < particles->n_particles(); ++i) {
    //     const auto& neighbors = particles->get_neighbors(i);
    //     std::cout << "Particle " << i << " has " << neighbors.size() << " neighbors: ";
    //     for (size_t j : neighbors) {
    //         std::cout << j << " ";
    //     }
    //     std::cout << "\n";
    // }
    particles->update_densities();
}

template <typename T>
void SPHSystemSolver2T<T>::accumulate_forces() {
    accumulate_pressure_forces();
    // print out particle 0 force for debugging
    auto particles = sphSystemData();
    auto& f = particles->get_forces();
    // std::cout << "Particle 0 force = (" << f[0].x << ", " << f[0].y << ")\n";

    accumulate_non_pressure_forces();
    // std::cout << "After non-pressure forces, Particle 0 force = (" << f[0].x << ", " << f[0].y << ")\n";
}

template <typename T>
void SPHSystemSolver2T<T>::accumulate_non_pressure_forces() {
    ParticleSystemSolver2D<T>::accumulate_forces();
    accumulate_viscosity_forces();
}

template <typename T>
void SPHSystemSolver2T<T>::accumulate_pressure_forces() {
    auto particles = sphSystemData();
    auto& x = particles->get_positions();
    auto& d = particles->get_densities();
    auto& p = particles->get_pressures();
    auto& f = particles->get_forces();
    compute_pressure();
    //std::cout << "After computing pressure, Particle 0 pressure = " << p[0] << "\n";
    accumulate_pressure_forces(x, d, p, f);
    //std::cout << "After accumulating pressure forces, Particle 0 force = (" << f[0].x << ", " << f[0].y << ")\n";
}

template <typename T>
void SPHSystemSolver2T<T>::accumulate_pressure_forces(
    const std::vector<cato::Vec2T<T>>& positions,
    const std::vector<T>& densities,
    const std::vector<T>& pressures,
    std::vector<cato::Vec2T<T>>& forces
) {
    auto particles = sphSystemData();
    size_t n_particles = particles->n_particles();
    const T mass_sq = particles->mass() * particles->mass();
    const SphSpikyKernal2<T> kernel(particles->radius());

    #pragma omp parallel for
    for (size_t i = 0; i < n_particles; ++i) {
        const auto& neighbors = particles->get_neighbors(i);
        for (size_t j : neighbors) {
            cato::Vec2T<T> rij = positions[i] - positions[j];
            T dist = rij.magnitude();
            if (dist > 0) {
                cato::Vec2T<T> dir = rij / dist;                
                T pressure_term = (pressures[i] / (densities[i] * densities[i]) +
                                   pressures[j] / (densities[j] * densities[j]));
                cato::Vec2T<T> pressure_force = -mass_sq * pressure_term * kernel.gradient(dist, dir);
                forces[i] += pressure_force;
            }
        }
    }  
}

template <typename T>
void SPHSystemSolver2T<T>::accumulate_viscosity_forces() {
    auto particles = sphSystemData();
    auto& x = particles->get_positions();
    auto& v = particles->get_velocities();
    auto& d = particles->get_densities();
    auto& f = particles->get_forces();

    const auto mass_sq = particles->mass() * particles->mass();
    const SphStdKernal2<T> kernel(particles->radius());
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

template <typename T>
void SPHSystemSolver2T<T>::compute_pressure(){
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
            0
        );
        // if (i == 0){
        //     std::cout << "Particle " << i << " density = " << d[i]
        //               << ", pressure = " << p[i] << "\n";
        // }
    }
}

template <typename T>
double SPHSystemSolver2T<T>::compute_pressure_from_eos(
    T density,
    T target_desnsity,
    T eos_scale,
    T eos_exponent,
    T negative_pressure_scale
) {
    // -1 is out of the std::power in the code/book, but in the equation??
    // pressure = k / eosExponent * (density / targetDensity - 1) ^ eosExponent
    T pressure = eos_scale / eos_exponent *
        (std::pow((density / target_desnsity), eos_exponent) - 1.0);
    if (pressure < 0) {
        pressure *= negative_pressure_scale;
    }
    return pressure;
}

template <typename T>
void SPHSystemSolver2T<T>::on_end_advance_timestep(double time_step_sec) {
    compute_psuedo_viscosity(time_step_sec);
}

template <typename T>
void SPHSystemSolver2T<T>::compute_psuedo_viscosity(double time_step_sec) {
    auto particles = sphSystemData();
    size_t numberOfParticles = particles->n_particles();
    auto& x = particles->get_positions();
    auto& v = particles->get_velocities();
    auto& d = particles->get_densities();

    const T mass = particles->mass();
    const SphSpikyKernal2<T> kernel(particles->radius());

    std::vector<cato::Vec2T<T>> smoothedVelocities(numberOfParticles);

    // TODO -> move to it's own function for building smoothed velocities
    #pragma omp parallel for
    for (size_t i = 0; i < numberOfParticles; ++i) {
        T weightSum = 0.0;
        cato::Vec2T<T> smoothedVelocity(0, 0);

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
        cato::Vec2T<T>& velocity = particles->get_velocity(i);
        const cato::Vec2T<T>& smoothed = smoothedVelocities[i];
        //velocity = lerp(velocity, smoothed, static_cast<T>(factor));
        velocity += (smoothed - velocity) * static_cast<T>(factor);
    }
}

template <typename T>
void SPHSystemSolver2T<T>::update_graphics() {
    for (size_t i = 0; i < sphSystemData()->n_particles(); ++i) {
		cato::Vec2T<T> pos = sphSystemData()->get_position(i);
		draw_solid_circle(pos.x, pos.y, sphSystemData()->radius() / static_cast<T>(10));
        draw_circle(pos.x, pos.y, sphSystemData()->radius());
	}
}

// explicit template instantiation
template class SPHSystemSolver2T<float>;
template class SPHSystemSolver2T<double>;