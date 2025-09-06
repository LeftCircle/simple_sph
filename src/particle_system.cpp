#include "particle_system.h"

ParticleSystem2D::ParticleSystem2D()
	: _n_particles(0), _radius(10.0), _mass(1.0) {
}

ParticleSystem2D::~ParticleSystem2D() {
}

size_t ParticleSystem2D::n_particles() const {
	return _n_particles;
}

void ParticleSystem2D::clear_particles() {
	_n_particles = 0;
	_positions.clear();
	_velocities.clear();
	_forces.clear();
}

void ParticleSystem2D::resize(const size_t n) {
	_n_particles = n;
	_positions.resize(_n_particles, cato::Vec2{ 0.0f, 0.0f });
	_velocities.resize(_n_particles, cato::Vec2{ 0.0f, 0.0f });
	_forces.resize(_n_particles, cato::Vec2{ 0.0f, 0.0f });
}

double ParticleSystem2D::radius() const {
	return _radius;
}

void ParticleSystem2D::set_radius(const double r) {
	_radius = r;
}

double ParticleSystem2D::mass() const {
	return _mass;
}

void ParticleSystem2D::set_mass(const double m) {
	_mass = m;
}


void ParticleSystem2D::randomize_particles(const float x_min, const float x_max,
	const float y_min, const float y_max) {
	for (size_t i = 0; i < _n_particles; ++i) {
		_positions[i].x = x_min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (x_max - x_min)));
		_positions[i].y = y_min + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (y_max - y_min)));
		_velocities[i].x = 0.0f;
		_velocities[i].y = 0.0f;
		_forces[i].x = 0.0f;
		_forces[i].y = 0.0f;
	}
}

void ParticleSystem2D::draw_circle(const float x, const float y, const float r) const {
	const int num_segments = 20;
	glColor3f(1.0f, 0.0f, 0.0f); // Red color
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < num_segments; ++i) {
		float theta = 2.0f * 3.1415926f * float(i) / float(num_segments);
		float dx = r * cosf(theta);
		float dy = r * sinf(theta);
		glVertex2f(x + dx, y + dy);
	}
	glEnd();
}
