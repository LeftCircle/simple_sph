#include "particle_system.h"



template<typename VecType>
ParticleSystem<VecType>::ParticleSystem()
	: _n_particles(0), _radius(10.0), _mass(1.0) {
	_particle_model.set_dimensions(cato::Vec3f(_radius));
}

template<typename VecType>
ParticleSystem<VecType>::ParticleSystem(size_t n_particles)
	: _radius(10.0), _mass(1.0) {
	resize(n_particles);
	_particle_model.set_dimensions(cato::Vec3f(_radius));
}

template<typename VecType>
ParticleSystem<VecType>::~ParticleSystem() {}

template<typename VecType>
size_t ParticleSystem<VecType>::n_particles() const {
	return _n_particles;
}

template<typename VecType>
void ParticleSystem<VecType>::clear_particles() {
	_n_particles = 0;
	_positions.clear();
	_velocities.clear();
	_forces.clear();
}

template<typename VecType>
void ParticleSystem<VecType>::resize(const size_t n) {
	_n_particles = n;
	_positions.resize(_n_particles, VecType());
	_velocities.resize(_n_particles, VecType());
	_forces.resize(_n_particles, VecType());
}

template<typename VecType>
auto ParticleSystem<VecType>::radius() const -> T {
	return _radius;
}

template<typename VecType>
void ParticleSystem<VecType>::set_radius(const T r) {
	_radius = r;
	_particle_model.set_dimensions(cato::Vec3f(_radius));
}

template<typename VecType>
auto ParticleSystem<VecType>::mass() const -> T {
	return _mass;
}

template<typename VecType>
void ParticleSystem<VecType>::set_mass(const T m) {
	_mass = m;
}

template<typename VecType>
void ParticleSystem<VecType>::randomize_particles(const VecType& blc, const VecType& trc) {
	for (size_t i = 0; i < _n_particles; ++i) {
		_positions[i].x = blc.x + static_cast<T>(rand()) / (static_cast<T>(RAND_MAX / (trc.x - blc.x)));
		_positions[i].y = blc.y + static_cast<T>(rand()) / (static_cast<T>(RAND_MAX / (trc.y - blc.y)));
		_velocities[i].x = 0.0f;
		_velocities[i].y = 0.0f;
		_forces[i].x = 0.0f;
		_forces[i].y = 0.0f;
		if constexpr (std::is_same<VecType, cato::Vec3T<typename VecType::value_type>>::value) {
			_positions[i].z = blc.z + static_cast<T>(rand()) / (static_cast<T>(RAND_MAX / (trc.z - blc.z)));
			_velocities[i].z = 0.0f;
			_forces[i].z = 0.0f;
		}
	}
}

template<typename VecType>
void ParticleSystem<VecType>::organize_particles_in_grid(
    const IntVec& origin,
    const IntVec& spacing,
    const IntVec& resolution
) {
	IntVec blc = origin - (resolution - IntVec{1}) * spacing / 2;
	if constexpr (std::is_same<VecType, cato::Vec2T<typename VecType::value_type>>::value) {
		int n_particles = resolution.x * resolution.y;
		resize(n_particles);
		for (int j = 0; j < resolution.y; ++j) {
			for (int i = 0; i < resolution.x; ++i) {
				int index = j * resolution.x + i;
				IntVec pos = blc + IntVec{ i * spacing.x, j * spacing.y };
				set_particle_position(index, VecType{ static_cast<T>(pos.x), static_cast<T>(pos.y) });
			}
		}
	} else {
		int n_particles = resolution.x * resolution.y * resolution.z;
		resize(n_particles);
		for (int k = 0; k < resolution.z; ++k) {
			for (int j = 0; j < resolution.y; ++j) {
				for (int i = 0; i < resolution.x; ++i) {
					int index = k * resolution.y * resolution.x + j * resolution.x + i;
					IntVec pos = blc + IntVec{ i * spacing.x, j * spacing.y, k * spacing.z };
					set_particle_position(index, VecType{ static_cast<T>(pos.x), static_cast<T>(pos.y), static_cast<T>(pos.z) });
				}
			}
		}
	}
}

template<typename VecType>
void ParticleSystem<VecType>::build_neighbor_lookup(
	const IntVec& resolution,
	double cell_size,
	const std::vector<VecType>& positions,
	const VecType& blc
) {
	_neighbor_lookup = std::make_shared<NeighborLookup>(resolution, cell_size);
	_neighbor_lookup->build(positions, blc);
}

template<typename VecType>
void ParticleSystem<VecType>::find_each_neighbor() {
	_neighbor_indices.resize(_n_particles);
	for (size_t i = 0; i < _n_particles; ++i) {
		_neighbor_indices[i].clear();
		const VecType& position = _positions[i];
		_neighbor_lookup->for_each_nearby_point(
			position,
			_radius,
			[&](size_t neighbor_index, const VecType&) {
				if (neighbor_index != i) {
					_neighbor_indices[i].push_back(neighbor_index);
				}
			}
		);
	}
}

// Explicit template instantiations
template class ParticleSystem<cato::Vec2f>;
template class ParticleSystem<cato::Vec2d>;
template class ParticleSystem<cato::Vec3d>;
template class ParticleSystem<cato::Vec3f>;