#ifndef PARTICLE_SYSTEM_SOLVER_H
#define PARTICLE_SYSTEM_SOLVER_H

#include <memory>
#include <vector>

#include "particle_system.h"
#include "physics_animation.h"
#include "collisions.h"

template <typename T>
class ParticleSystemSolver2D : public PhysicsAnimation {

public:
    ParticleSystemSolver2D();
    ParticleSystemSolver2D(size_t n_particles);
    virtual ~ParticleSystemSolver2D();
    
protected:
   	// Protected constructor for derived classes to set the particle type
    ParticleSystemSolver2D(const std::shared_ptr<ParticleSystem2D<T>> particle_system);
    void set_particle_system(const std::shared_ptr<ParticleSystem2D<T>> particle_system);
    
    void on_update(double delta) override;

    void handle_collisions(double time_step_sec) override;
    void apply_constraints(double time_step_sec) override;
    void update_graphics() override;
    
    virtual void accumulate_forces();
    virtual void on_begin_advance_timestep(double time_step_sec) {}
    virtual void on_end_advance_timestep(double time_step_sec) {}
    std::shared_ptr<ParticleSystem2D<T>> _particle_system;

private:
    void begin_advance_timestep(double time_step_sec);
    void end_advance_timestep(double time_step_sec);
    void integrate(double time_step_sec) override;
    
    
    std::vector<cato::Vec2T<T>> _new_positions;
    std::vector<cato::Vec2T<T>> _new_velocities;    
};


using ParticleSystemSolver2Dd = ParticleSystemSolver2D<double>;
using ParticleSystemSolver2Df = ParticleSystemSolver2D<float>;

using ParticleSystemSolver2DPtrd = std::shared_ptr<ParticleSystemSolver2Dd>;
using ParticleSystemSolver2DfPtr = std::shared_ptr<ParticleSystemSolver2Df>;

#endif