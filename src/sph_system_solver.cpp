#include "sph_system_solver.h"

template <typename T>
SPHSystemSolver2T<T>::SPHSystemSolver2T() : ParticleSystemSolver2D<T>() {}

template <typename T>
SPHSystemSolver2T<T>::SPHSystemSolver2T(size_t n_particles) : ParticleSystemSolver2D<T>(n_particles) {}

template <typename T>
SPHSystemSolver2T<T>::~SPHSystemSolver2T() {}


template <typename T>
void SPHSystemSolver2T<T>::on_begin_advance_timestep(double time_step_sec) {
    auto particles = sphSystemData();
    particles->build_neighbor_lookup(50, 50);
    particles->find_each_neighbor();
    particles->update_densities();
}

template <typename T>
void SPHSystemSolver2T<T>::accumulate_forces(double time_step_sec) {
    accumulate_pressure_forces(time_step_sec);
    accumulate_non_pressure_forces(time_step_sec);
}

template <typename T>
void SPHSystemSolver2T<T>::accumulate_non_pressure_forces(double time_step_sec) {
    ParticleSystemSolver2D<T>::accumulate_forces(time_step_sec);
    accumulate_viscosity_forces();
}

template <typename T>
void SPHSystemSolver2T<T>::accumulate_pressure_forces(double time_step_sec) {
    auto particles = sphSystemData();
    auto& x = particles->get_positions();
    auto& d = particles->get_densities();
    auto& p = particles->get_pressures();
    auto& f = particles->get_forces();
    compute_pressure();
    accumulate_pressure_forces(x, d, p, f);
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
    


}

template <typename T>
void SPHSystemSolver2T<T>::compute_pressure(){
    auto particles = sphSystemData();
    auto& d = particles->get_densities();
    auto& p = particles->get_pressures();
    size_t n_particles = particles->n_particles();
    T target_density = particles->get_target_density();
    T eos_scale = static_cast<T>(SPEED_OF_SOUND * SPEED_OF_SOUND * target_density) / this->_eos_exponent;
    #pragma omp parallel for
    for (size_t i = 0; i < n_particles; ++i) {
        p[i] = compute_pressure_from_eos(
            d[i],
            target_density,
            eos_scale,
            this->_eos_exponent,
            0
        );
    }
}

template <typename T>
void SPHSystemSolver2T<T>::on_end_advance_timestep(double time_step_sec) {
    compute_psuedo_viscosity();
}

template <typename T>
double SPHSystemSolver2T<T>::compute_pressure_from_eos(
    T density,
    T target_desnsity,
    T eos_scale,
    T eos_exponent,
    T negative_pressure_scale
) {
    double pressure = eos_scale / eos_exponent *
        (std::pow(density / target_desnsity, eos_exponent) - 1.0);
    if (pressure < 0) {
        pressure *= negative_pressure_scale;
    }
    return pressure;
}


// explicit template instantiation
template class SPHSystemSolver2T<float>;
template class SPHSystemSolver2T<double>;