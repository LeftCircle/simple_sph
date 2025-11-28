#ifndef PARTICLE_SYSTEM_SOLVER_H
#define PARTICLE_SYSTEM_SOLVER_H


#include "particle_system.h"
#include "physics_animation.h"

// TODO -> move the drawing logic from the particle system data to here
template <typename T>
class ParticleSystemSolver2D : public PhysicsAnimation {

public:
    ParticleSystemSolver2D();
    virtual ~ParticleSystemSolver2D();

protected:
    void on_update(const float delta) override;

    virtual void accumulate_forces(double time_step_sec);

    void handle_collisions(double time_step_sec) override;

};


#endif