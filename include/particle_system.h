#ifndef PARTICLE_SYSTEM_ANIMATION_H
#define PARTICLE_SYSTEM_ANIMATION_H


#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <array>
#include <memory>
#include <math.h>

#include "vector.h"
#include "neighbor_lookup_hashgrid2D.h"
#include "neighbor_lookup_hashgrid.h"
#include "shapes.h"

template<typename VecType>
class ParticleSystem {
public:
	using T = decltype(VecType().x);
	using IntVec = std::conditional_t<std::is_same<VecType, cato::Vec2T<T>>::value, cato::Vec2i, cato::Vec3i>;
	using NeighborLookup = std::conditional_t<
		std::is_same<VecType, cato::Vec2T<T>>::value,
		PointNeighborLookupHashGrid2<T>,
		PointNeighborLookupHashGrid3<T>
	>;

	ParticleSystem();
	ParticleSystem(size_t n_particles);
	~ParticleSystem();
	
	size_t n_particles() const;
	void clear_particles();
	
	// TO DO -> pack all of the appropriate data into one array
	// so that we don't need a resize function for each inherited class
	virtual void resize(const size_t n);
	
	void randomize_particles(const VecType& blc, const VecType& trc);
	
	void organize_particles_in_grid(
		const IntVec& origin,
		const IntVec& spacing,
		const IntVec& resolution
	);

	T radius() const;
	void set_radius(const T r);

	T mass() const;
	void set_mass(const T m);
	const VecType& get_position(int particle_n) const { return _positions[particle_n]; };
	VecType& get_position(int particle_n) { return _positions[particle_n]; };
	
	void set_particle_position(int particle_n, const VecType& position) {
		_positions[particle_n] = position;
	}
	
	const VecType& get_velocity(int particle_n) const { return _velocities[particle_n]; };
	VecType& get_velocity(int particle_n) { return _velocities[particle_n]; };
	
	const VecType& get_force(int particle_n) const { return _forces[particle_n]; };
	VecType& get_force(int particle_n) { return _forces[particle_n]; };

	std::vector<VecType>& get_positions() { return _positions; }
	const std::vector<VecType>& get_positions() const { return _positions; }
	
	std::vector<VecType>& get_velocities() { return _velocities; }
	const std::vector<VecType>& get_velocities() const { return _velocities; }
	
	std::vector<VecType>& get_forces() { return _forces; }
	const std::vector<VecType>& get_forces() const { return _forces; }
	
	const std::vector<size_t>& get_neighbors(size_t particle_n) const {
		return _neighbor_indices[particle_n];
	}

	void clear_velocities() {
		for (size_t i = 0; i < _n_particles; ++i) {
			_velocities[i] *= static_cast<T>(0);
		}
	}

	// Neighbor lookup structures
	void build_neighbor_lookup(
		const IntVec& resolution,
		double cell_size,
		const std::vector<VecType>& positions,
		const VecType& blc
	);
	void find_each_neighbor();
	void bind_vao() const { _particle_model.bind_vao(); }
	void bind_particle_model_verts() { _particle_model.bind_verts(); }
	void draw_particle_model() const { _particle_model.draw(); }

	
protected:
	size_t _n_particles;
	T _radius = static_cast<T>(1.0);
	T _mass = static_cast<T>(1.0);

	// Basic vectors containing the x,y positions, velocities, and forces
	std::vector<VecType> _positions;
	std::vector<VecType> _velocities;
	std::vector<VecType> _forces;
	// Neighbor lookup structure
	std::shared_ptr<NeighborLookup> _neighbor_lookup;
	std::vector<std::vector<size_t>> _neighbor_indices;

	SolidBox3D _particle_model{cato::Vec3f(0), cato::Vec3f(10.0)};


};

template<typename VecType>
using ParticleSystemPtr = std::shared_ptr<ParticleSystem<VecType>>;

#endif // PARTICLE_SYSTEM_ANIMATION_H