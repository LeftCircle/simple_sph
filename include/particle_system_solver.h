#ifndef PARTICLE_SYSTEM_SOLVER_H
#define PARTICLE_SYSTEM_SOLVER_H

#include <memory>
#include <vector>

#include "particle_system.h"
#include "physics_animation.h"
#include "collisions.h"

const double SPEED_OF_SOUND = 1481.0; // Speed of sound in water in m/s

// TODO -> move the drawing logic from the particle system data to here
template <typename T>
class ParticleSystemSolver2D : public PhysicsAnimation {

public:
    ParticleSystemSolver2D();
    ParticleSystemSolver2D(size_t n_particles);
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
    T _eos_exponent = static_cast<T>(7.0);
    
};


using ParticleSystemSolver2Dd = ParticleSystemSolver2D<double>;
using ParticleSystemSolver2Df = ParticleSystemSolver2D<float>;

using ParticleSystemSolver2DPtrd = std::shared_ptr<ParticleSystemSolver2Dd>;
using ParticleSystemSolver2DfPtr = std::shared_ptr<ParticleSystemSolver2Df>;

#endif