#include "particle_system.h"

template<typename T>
ParticleSystem2D<T>::ParticleSystem2D()
	: _n_particles(0), _radius(10.0), _mass(1.0) {
}

template<typename T>
ParticleSystem2D<T>::ParticleSystem2D(size_t n_particles)
	: _n_particles(n_particles), _radius(10.0), _mass(1.0) {
	_positions.resize(_n_particles, cato::Vec2T<T>{ 0.0, 0.0 });
	_velocities.resize(_n_particles, cato::Vec2T<T>{ 0.0, 0.0 });
	_forces.resize(_n_particles, cato::Vec2T<T>{ 0.0, 0.0 });
}

template<typename T>
ParticleSystem2D<T>::~ParticleSystem2D() {
}

template<typename T>
size_t ParticleSystem2D<T>::n_particles() const {
	return _n_particles;
}

template<typename T>
void ParticleSystem2D<T>::clear_particles() {
	_n_particles = 0;
	_positions.clear();
	_velocities.clear();
	_forces.clear();
}

template<typename T>
void ParticleSystem2D<T>::resize(const size_t n) {
	_n_particles = n;
	_positions.resize(_n_particles, cato::Vec2T<T>{ 0.0, 0.0 });
	_velocities.resize(_n_particles, cato::Vec2T<T>{ 0.0, 0.0 });
	_forces.resize(_n_particles, cato::Vec2T<T>{ 0.0, 0.0 });
}

template<typename T>
T ParticleSystem2D<T>::radius() const {
	return _radius;
}

template<typename T>
void ParticleSystem2D<T>::set_radius(const T r) {
	_radius = r;
}

template<typename T>
T ParticleSystem2D<T>::mass() const {
	return _mass;
}

template<typename T>
void ParticleSystem2D<T>::set_mass(const T m) {
	_mass = m;
}


template<typename T>
void ParticleSystem2D<T>::randomize_particles(const T x_min, const T x_max,
	const T y_min, const T y_max) {
	for (size_t i = 0; i < _n_particles; ++i) {
		_positions[i].x = x_min + static_cast<T>(rand()) / (static_cast<T>(RAND_MAX / (x_max - x_min)));
		_positions[i].y = y_min + static_cast<T>(rand()) / (static_cast<T>(RAND_MAX / (y_max - y_min)));
		_velocities[i].x = 0.0f;
		_velocities[i].y = 0.0f;
		_forces[i].x = 0.0f;
		_forces[i].y = 0.0f;
	}
}

template<typename T>
void ParticleSystem2D<T>::place_particle_grid(const cato::Vec2T<T>& origin,
	const cato::Vec2T<T>& spacing,
	const cato::Vec2i& resolution) {
	
	int n_placed = 0;
	for (int j = 0; j < resolution.y; ++j) {
		for (int i = 0; i < resolution.x; ++i) {
			if (n_placed < static_cast<int>(_n_particles)) {
				_positions[n_placed].x = origin.x + i * spacing.x;
				_positions[n_placed].y = origin.y + j * spacing.y;
				++n_placed;
			}
		}
	}
}

template<typename T>
void ParticleSystem2D<T>::build_neighbor_lookup(int resolution_x, int resolution_y, double cell_size) {
	_neighbor_lookup = std::make_shared<PointNeighborLookupHashGrid2<T>>(
		cato::Vec2i{resolution_x, resolution_y}, cell_size);
	
	_neighbor_lookup->build(_positions);
}

template<typename T>
void ParticleSystem2D<T>::find_each_neighbor() {
	_neighbor_indices.resize(_n_particles);
	for (size_t i = 0; i < _n_particles; ++i) {
		_neighbor_indices[i].clear();
		const cato::Vec2T<T>& position = _positions[i];
		_neighbor_lookup->for_each_nearby_point(
			position,
			_radius,
			[&](size_t neighbor_index, const cato::Vec2T<T>&) {
				if (neighbor_index != i) {
					_neighbor_indices[i].push_back(neighbor_index);
				}
			}
		);
	}
}

// Explicit template instantiations
template class ParticleSystem2D<float>;
template class ParticleSystem2D<double>;
