#ifndef PARTICLE_SYSTEM_ANIMATION_H
#define PARTICLE_SYSTEM_ANIMATION_H

#include <GL/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <array>
#include <memory>
#include <math.h>


#include "physics_animation.h"

class ParticleSystem2D {
public:
	ParticleSystem2D();
	~ParticleSystem2D();
	
	size_t n_particles() const;
	void clear_particles();
	void resize(const size_t n);
	void randomize_particles(const float x_min, const float x_max,
		const float y_min, const float y_max);

	double radius() const;
	void set_radius(const double r);
	void draw_circle(const float x, const float y, const float r) const;

	double mass() const;
	void set_mass(const double m);
	std::array<float, 2> get_position(int particle_n) const;
	std::array<float, 2> get_position(int particle_n);
	std::array<float, 2> get_velocity(int particle_n) const;
	std::array<float, 2> get_velocity(int particle_n);
	std::array<float, 2> get_force(int particle_n) const;
	std::array<float, 2> get_force(int particle_n);

private:
	size_t _n_particles;
	double _radius;
	double _mass;

	// Basic vectors containing the x,y positions, velocities, and forces
	std::vector<float> _positions;
	std::vector<float> _velocities;
	std::vector<float> _forces;

};
typedef std::shared_ptr<ParticleSystem2D> ParticleSystem2DPtr;

#endif // PARTICLE_SYSTEM_ANIMATION_H