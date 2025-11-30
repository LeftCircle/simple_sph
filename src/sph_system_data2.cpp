#include "sph_system_data2.h"



template<typename T>
SPHSystemData2<T>::SPHSystemData2() : ParticleSystem2D<T>() {
    _densities.resize(this->n_particles(), static_cast<T>(0));
}

template<typename T>
SPHSystemData2<T>::SPHSystemData2(size_t n_particles) : ParticleSystem2D<T>(n_particles) {
    _densities.resize(this->n_particles(), static_cast<T>(0));
}

template<typename T>
SPHSystemData2<T>::~SPHSystemData2() {}


template<typename T>
void SPHSystemData2<T>::resize(const size_t n) {
    ParticleSystem2D<T>::resize(n);
    _densities.resize(this->n_particles(), static_cast<T>(0));
    _pressures.resize(this->n_particles(), static_cast<T>(0));
}

template<typename T>
void SPHSystemData2<T>::update_densities() {
    // This will update densities for all particles
    auto d = this->get_densities();
    auto p = this->get_positions();
    SphSpikyKernal2<T> kernel(this->radius());
    size_t n_particles = this->n_particles();
    #pragma omp parallel for
    for (size_t i = 0; i < n_particles; i++) {
        T density = static_cast<T>(0);
        this->_neighbor_lookup->for_each_nearby_point(
            p[i],
            this->_radius,
            [&](size_t j, const cato::Vec2T<T>& neighborPosition) {
                T distance = (p[i] - neighborPosition).magnitude();
                density += this->_mass * kernel(distance);
            }
        );
        // add our own particle contribution
        density += this->_mass * kernel(0);
        d[i] = density;
    }
}

template<typename T>
T SPHSystemData2<T>::sum_of_nearby_kernal(const cato::Vec2T<T>& position) const {
    T sum = static_cast<T>(0);
    SphSpikyKernal2<T> kernel(this->radius());
    this->_neighbor_lookup->for_each_nearby_point(
        position,
        this->_radius,
        [&](size_t i, const cato::Vec2T<T>& neighborPosition) {
            T distance = (position - neighborPosition).magnitude();
            sum += kernel(distance);
        }
    );
    return sum;
}

template<typename T>
cato::Vec2T<T> SPHSystemData2<T>::interpolate(const cato::Vec2T<T>& position,
    const std::vector<cato::Vec2T<T>>& values) const {
    
    cato::Vec2T<T> result{ 0, 0 };
    auto d = this->get_densities();
    SphSpikyKernal2<T> kernel(this->radius());
    this->_neighbor_lookup->for_each_nearby_point(
        position,
        this->_radius,
        [&](size_t i, const cato::Vec2T<T>& neighborPosition) {
            T distance = (position - neighborPosition).magnitude();
            T weight = this->_mass / d[i] * kernel(distance);
            result += values[i] * weight;
        }
    );

    // TODO -> Can probably use sum of nearby kernal then multipy result by mass / density_sum
    return result;
}

template<typename T>
cato::Vec2T<T> SPHSystemData2<T>::gradient_at(size_t particle_n,
    const std::vector<T>& values) const {

    cato::Vec2T<T> result(0, 0);
    auto d = this->get_densities();
    auto p = this->get_positions();
    auto origin = p[particle_n];
    SphSpikyKernal2<T> kernel(this->_radius);
    const auto& neighbors = this->_neighbor_indices[particle_n];
    
    // TODO -> Create a for each neighbor function
    for (size_t j : neighbors) {
        cato::Vec2T<T> vec_to = origin - p[j];
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

template<typename T>
T SPHSystemData2<T>::laplacian_at(size_t particle_n,
    const std::vector<T>& values) const {
    
    T result = 0.0;
    auto d = this->get_densities();
    auto p = this->get_positions();
    auto origin = p[particle_n];
    SphStdKernal2<T> kernel(this->_radius);
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
template class SPHSystemData2<float>;
template class SPHSystemData2<double>;

