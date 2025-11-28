#include "sph_system_data2.h"



template<typename T>
SPHSystemData2<T>::SPHSystemData2() : ParticleSystem2D<T>() {
    _densities.resize(this->n_particles(), static_cast<T>(0));
}

template<typename T>
SPHSystemData2<T>::~SPHSystemData2() {}

template<typename T>
cato::Vec2T<T> SPHSystemData2<T>::interpolate(const cato::Vec2T<T>& position,
    const std::vector<cato::Vec2T<T>>& values) const {
    
    cato::Vec2T<T> result{ 0, 0 };
    auto d = get_densities();
    SphSpikyKernal2 kernel(this->radius());
    _neighbor_lookup->for_each_nearby_point(
        position,
        _radius,
        [&](size_t i, const cato::Vec2T<T>& neighborPosition) {
            T distance = (position - neighborPosition).magnitude();
            T weight = _mass / d[i] * kernel(distance);
            result += values[i] * weight;
        }
    )
    return result;
}


// Explicit template instantiations
template class SPHSystemData2<float>;
template class SPHSystemData2<double>;

