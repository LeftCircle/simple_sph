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
    
    std::shared_ptr<ParticleSystem2D<T>> particleSystem() {
        return _particle_system;
    }
    void update_graphics() override;

    T get_gravity() const { return _gravity; }
    void set_gravity(T g) { _gravity = g; }
    void add_interaction_force(const cato::Vec2T<T>& position, T radius, T strength, T direction = static_cast<T>(1));

    
protected:
    // Protected constructor for derived classes to set the particle type
    ParticleSystemSolver2D(const std::shared_ptr<ParticleSystem2D<T>> particle_system);
    void set_particle_system(const std::shared_ptr<ParticleSystem2D<T>> particle_system);

    void on_update(double delta) override;

    void handle_collisions(double time_step_sec) override;
    void apply_constraints(double time_step_sec) override;

    virtual void accumulate_forces();
    virtual void apply_interaction_forces();
    virtual void on_begin_advance_timestep(double time_step_sec) {}
    virtual void on_end_advance_timestep(double time_step_sec) {}
    std::shared_ptr<ParticleSystem2D<T>> _particle_system;
    T _gravity = static_cast<T>(-9.8);

private:
    void begin_advance_timestep(double time_step_sec);
    void end_advance_timestep(double time_step_sec);
    void integrate(double time_step_sec) override;

    T _interactive_force_strength = static_cast<T>(1000);
    T _interactive_force_radius = static_cast<T>(50);
    cato::Vec2T<T> _interactive_force_position = cato::Vec2T<T>(0, 0);
    T _interactive_force_direction = static_cast<T>(1);
    
    std::vector<cato::Vec2T<T>> _new_positions;
    std::vector<cato::Vec2T<T>> _new_velocities;    
};


using ParticleSystemSolver2Dd = ParticleSystemSolver2D<double>;
using ParticleSystemSolver2Df = ParticleSystemSolver2D<float>;

using ParticleSystemSolver2DPtrd = std::shared_ptr<ParticleSystemSolver2Dd>;
using ParticleSystemSolver2DfPtr = std::shared_ptr<ParticleSystemSolver2Df>;

#endif