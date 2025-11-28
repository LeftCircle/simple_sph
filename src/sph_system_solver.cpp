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
void SPHSystemSolver2T<T>::accumulate_pressure_forces(std::vector<cato::Vec2T<T>>& positions,
    std::vector<T>& densities,
    std::vector<T>& pressures,
    std::vector<cato::Vec2T<T>>& forces) {

}

template <typename T>
void SPHSystemSolver2T<T>::compute_pressure(){

}

// explicit template instantiation
template class SPHSystemSolver2T<float>;
template class SPHSystemSolver2T<double>;