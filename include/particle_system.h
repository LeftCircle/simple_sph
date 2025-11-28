#ifndef PARTICLE_SYSTEM_ANIMATION_H
#define PARTICLE_SYSTEM_ANIMATION_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <array>
#include <memory>
#include <math.h>

#include "vector.h"
#include "neighbor_lookup_hashgrid2D.h"

template<typename T>
class ParticleSystem2D {
public:
	ParticleSystem2D();
	~ParticleSystem2D();
	
	size_t n_particles() const;
	void clear_particles();
	void resize(const size_t n);
	void randomize_particles(const T x_min, const T x_max,
		const T y_min, const T y_max);

	T radius() const;
	void set_radius(const T r);
	void draw_circle(const T x, const T y, const T r) const;

	T mass() const;
	void set_mass(const T m);
	const cato::Vec2T<T>& get_position(int particle_n) const { return _positions[particle_n]; };
	cato::Vec2T<T>& get_position(int particle_n) { return _positions[particle_n]; };
	
	const cato::Vec2T<T>& get_velocity(int particle_n) const { return _velocities[particle_n]; };
	cato::Vec2T<T>& get_velocity(int particle_n) { return _velocities[particle_n]; };
	
	const cato::Vec2T<T>& get_force(int particle_n) const { return _forces[particle_n]; };
	cato::Vec2T<T>& get_force(int particle_n) { return _forces[particle_n]; };

	const std::vector<cato::Vec2T<T>>& get_positions() const { return _positions; }
	const std::vector<cato::Vec2T<T>>& get_velocities() const { return _velocities; }
	const std::vector<cato::Vec2T<T>>& get_forces() const { return _forces; }
	std::vector<cato::Vec2T<T>>& get_positions() { return _positions; }
	std::vector<cato::Vec2T<T>>& get_velocities() { return _velocities; }
	std::vector<cato::Vec2T<T>>& get_forces() { return _forces; }
	
	// Neighbor lookup structures
	void build_neighbor_lookup(int resolution_x, int resolution_y);
	void find_each_neighbor();

	
protected:
	size_t _n_particles;
	T _radius;
	T _mass;

	// Basic vectors containing the x,y positions, velocities, and forces
	std::vector<cato::Vec2T<T>> _positions;
	std::vector<cato::Vec2T<T>> _velocities;
	std::vector<cato::Vec2T<T>> _forces;
	// Neighbor lookup structure
	std::shared_ptr<PointNeighborLookupHashGrid2<T>> _neighbor_lookup;
	std::vector<std::vector<size_t>> _neighbor_indices;

};

template<typename T>
using ParticleSystem2DPtr = std::shared_ptr<ParticleSystem2D<T>>;

#endif // PARTICLE_SYSTEM_ANIMATION_H