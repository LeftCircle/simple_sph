#ifndef SPH_ANIMATION_H
#define SPH_ANIMATION_H

#include "GL/glew.h"
#include "GL/freeglut.h"
#include <iostream>

#include "physics_animation.h"
#include "particle_system.h"

class SPHAnimation : public PhysicsAnimation {
public:
	SPHAnimation();
	~SPHAnimation();

	SPHAnimation(const int num_particles);

	ParticleSystem2DPtr get_particle_system() const { return _particle_system; }
	ParticleSystem2DPtr get_particle_system() { return _particle_system; }

protected:
	void on_update(const float delta) override;
	void accumulate_forces() override;
	void integrate(const float delta) override;
	void handle_collisions() override;
	void apply_constraints() override;
	void update_graphics() override;

private:
	ParticleSystem2DPtr _particle_system;


};




typedef std::shared_ptr<SPHAnimation> SPHAnimationPtr;

#endif // SPH_ANIMATION_H