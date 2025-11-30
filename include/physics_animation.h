#ifndef PHYSICS_ANIMATION_H
#define PHYSICS_ANIMATION_H

#include <memory>

class PhysicsAnimation {
public:
	PhysicsAnimation();
	~PhysicsAnimation();

	void update(double delta);
	virtual void update_graphics() = 0;

protected:
	virtual void on_update(double delta) = 0;
	virtual void accumulate_forces() = 0;
	virtual void integrate(double delta) = 0;
	virtual void handle_collisions(double delta) = 0;
	virtual void apply_constraints(double delta) = 0;
	
};


typedef std::shared_ptr<PhysicsAnimation> PhysicsAnimationPtr;

#endif // PHYSICS_ANIMATION_H