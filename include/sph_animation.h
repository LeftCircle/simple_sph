#ifndef SPH_ANIMATION_H
#define SPH_ANIMATION_H

#include "GL/glew.h"
#include "GL/freeglut.h"
#include <iostream>

#include "physics_animation.h"
#include "particle_system.h"
#include "collisions.h"

class SPHAnimation : public PhysicsAnimation {
public:
	SPHAnimation();
	~SPHAnimation();

	SPHAnimation(const int num_particles);

	ParticleSystem2DPtr<double> get_particle_system() const { return _particle_system; }
	ParticleSystem2DPtr<double> get_particle_system() { return _particle_system; }
	void resize_particle_system(size_t n);
	void update_graphics() override;

protected:
	void on_update(const float delta) override;
	void accumulate_forces() override;
	void integrate(const float delta) override;
	void handle_collisions(const float delta) override;
	void apply_constraints(const float delta) override;

private:
	ParticleSystem2DPtr<double> _particle_system;

	void _update_to_new_state();

};




typedef std::shared_ptr<SPHAnimation> SPHAnimationPtr;

#endif // SPH_ANIMATION_H