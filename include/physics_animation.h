#ifndef PHYSICS_ANIMATION_H
#define PHYSICS_ANIMATION_H

#include <memory>

class PhysicsAnimation {
public:
	PhysicsAnimation();
	~PhysicsAnimation();

	void update(const float delta);

protected:
	virtual void on_update(const float delta) = 0;
	virtual void accumulate_forces() = 0;
	virtual void integrate(const float delta) = 0;
	virtual void handle_collisions(const float delta) = 0;
	virtual void apply_constraints(const float delta) = 0;
	virtual void update_graphics() = 0;
	
};


typedef std::shared_ptr<PhysicsAnimation> PhysicsAnimationPtr;

#endif // PHYSICS_ANIMATION_H