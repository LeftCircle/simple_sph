#include "sph_system_data2.h"



template<typename VecType>
SPHSystemData<VecType>::SPHSystemData() : ParticleSystem<VecType>() {
    _densities.resize(this->n_particles(), static_cast<T>(0));
}

template<typename VecType>
SPHSystemData<VecType>::SPHSystemData(size_t n_particles) : ParticleSystem<VecType>(n_particles) {
    _densities.resize(this->n_particles(), static_cast<T>(0));
}

template<typename VecType>
SPHSystemData<VecType>::~SPHSystemData() {}


template<typename VecType>
void SPHSystemData<VecType>::resize(const size_t n) {
    ParticleSystem<VecType>::resize(n);
    _densities.resize(this->n_particles(), static_cast<T>(0));
    _pressures.resize(this->n_particles(), static_cast<T>(0));
}

template<typename VecType>
void SPHSystemData<VecType>::update_densities(const std::vector<VecType>& positions) {
    // This will update densities for all particles
    auto& d = this->get_densities();
    Kernel kernel(this->radius());
    size_t n_particles = this->n_particles();
    
    #pragma omp parallel for
    for (size_t i = 0; i < n_particles; i++) {
        T density = static_cast<T>(0);
        const auto& neighbors = this->_neighbor_indices[i];
        for (size_t j : neighbors) {
            T distance = positions[i].distance_to(positions[j]);
            density += this->_mass * kernel(distance);
        }
        // Add own particle density
        density += this->_mass * kernel(0);
        d[i] = density;
    }
}

template<typename VecType>
auto SPHSystemData<VecType>::sum_of_nearby_kernal(const VecType& position) const -> T {
    T sum = static_cast<T>(0);
    Kernel kernel(this->radius());
    this->_neighbor_lookup->for_each_nearby_point(
        position,
        this->_radius,
        [&](size_t i, const VecType& neighborPosition) {
            T distance = (position - neighborPosition).magnitude();
            sum += kernel(distance);
        }
    );
    return sum;
}

template<typename VecType>
VecType SPHSystemData<VecType>::interpolate(const VecType& position,
    const std::vector<VecType>& values) const {
    
    VecType result;
    auto& d = this->get_densities();
    Kernel kernel(this->radius());
    this->_neighbor_lookup->for_each_nearby_point(
        position,
        this->_radius,
        [&](size_t i, const VecType& neighborPosition) {
            T distance = (position - neighborPosition).magnitude();
            T weight = this->_mass / d[i] * kernel(distance);
            result += values[i] * weight;
        }
    );

    // TODO -> Can probably use sum of nearby kernal then multipy result by mass / density_sum
    return result;
}

template<typename VecType>
VecType SPHSystemData<VecType>::gradient_at(size_t particle_n,
    const std::vector<T>& values) const {

    VecType result(0);
    auto& d = this->get_densities();
    auto& p = this->get_positions();
    auto origin = p[particle_n];
    Kernel kernel(this->_radius);
    const auto& neighbors = this->_neighbor_indices[particle_n];
    
    // TODO -> Create a for each neighbor function
    for (size_t j : neighbors) {
        VecType vec_to = origin - p[j];
        T distance = vec_to.magnitude();
        if (distance > 0) {
            vec_to = vec_to / distance;
            auto density_avg = (values[particle_n] / (d[particle_n] * d[particle_n]) +
                                values[j] / (d[j] * d[j]));
            density_avg *= d[particle_n] * this->_mass;
            auto grad_weight = kernel.gradient(distance, vec_to);
            result += density_avg * grad_weight;
        }
    }
    return result;
}

template<typename VecType>
auto SPHSystemData<VecType>::laplacian_at(size_t particle_n,
    const std::vector<T>& values) const -> T {
    
    T result = 0.0;
    auto& d = this->get_densities();
    auto& p = this->get_positions();
    auto origin = p[particle_n];
    Kernel kernel(this->_radius);
    const auto& neighbors = this->_neighbor_indices[particle_n];

    // TODO -> Create a for each neighbor function
    for (size_t j : neighbors) {
        auto density_term = (values[j] - values[particle_n]) / d[j];
        auto kernal_laplacian = kernel.second_derivative((origin - p[j]).magnitude());
        result += this->_mass * density_term * kernal_laplacian;;
    }
    return result;
}


// Explicit template instantiations
template class SPHSystemData<cato::Vec2f>;
template class SPHSystemData<cato::Vec2d>;

