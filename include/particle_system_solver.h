#ifndef PARTICLE_SYSTEM_SOLVER_H
#define PARTICLE_SYSTEM_SOLVER_H

#include <memory>
#include <vector>

#include "particle_system.h"
#include "physics_animation.h"
#include "collisions.h"

// TODO -> move the drawing logic from the particle system data to here
template <typename T>
class ParticleSystemSolver2D : public PhysicsAnimation {

public:
    ParticleSystemSolver2D();
    virtual ~ParticleSystemSolver2D();
    
protected:
    void on_update(double delta) override;

    void handle_collisions(double time_step_sec) override;
    
    virtual void accumulate_forces(double time_step_sec);
    virtual void on_begin_advance_timestep(double time_step_sec) {}
    virtual void on_end_advance_timestep(double time_step_sec) {}

private:
    void begin_advance_timestep(double time_step_sec);
    void end_advance_timestep(double time_step_sec);
    void integrate(double time_step_sec) override;
    
    std::shared_ptr<ParticleSystem2D<T>> _particle_system;
    
    std::vector<cato::Vec2T<T>> _new_positions;
    std::vector<cato::Vec2T<T>> _new_velocities;    
    
};


using ParticleSystemSolver2Dd = ParticleSystemSolver2D<double>;
using ParticleSystemSolver2Df = ParticleSystemSolver2D<float>;

using ParticleSystemSolver2DPtrd = std::shared_ptr<ParticleSystemSolver2Dd>;
using ParticleSystemSolver2DfPtr = std::shared_ptr<ParticleSystemSolver2Df>;

#endif