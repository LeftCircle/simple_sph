#ifndef PARTICLE_SYSTEM_SOLVER_H
#define PARTICLE_SYSTEM_SOLVER_H

#include <memory>
#include <vector>

#include "particle_system.h"
#include "physics_animation.h"
#include "collisions.h"
#include "shapes.h"

template <typename VecType>
class ParticleSystemSolver : public PhysicsAnimation {
public:
	using T = decltype(VecType().x);
    using BoxBounds = Box<VecType>;

    ParticleSystemSolver();
    ParticleSystemSolver(size_t n_particles);
    virtual ~ParticleSystemSolver();
    
    std::shared_ptr<ParticleSystem<VecType>> particleSystem() {
        return _particle_system;
    }
    void update_graphics() override;

    T get_gravity() const { return _gravity; }
    void set_gravity(T g) { _gravity = g; }
    void add_interaction_force(const VecType& position, T radius, T strength, T direction = static_cast<T>(1));
    void set_boundary_box(const BoxBounds& box) { _boundary_box = box; }

    void bind_boundary_box_verts() {
        _boundary_box.bind_verts();
    }
    
protected:
    // Protected constructor for derived classes to set the particle type
    ParticleSystemSolver(const std::shared_ptr<ParticleSystem<VecType>> particle_system);
    void set_particle_system(const std::shared_ptr<ParticleSystem<VecType>> particle_system);

    void on_update(double delta) override;

    void handle_collisions(double time_step_sec) override;
    void apply_constraints(double time_step_sec) override;

    virtual void accumulate_forces();
    virtual void apply_interaction_forces();
    virtual void on_begin_advance_timestep(double time_step_sec) {}
    virtual void on_end_advance_timestep(double time_step_sec) {}
    std::shared_ptr<ParticleSystem<VecType>> _particle_system;
    T _gravity = static_cast<T>(-9.8);
    BoxBounds _boundary_box;

private:
    void begin_advance_timestep(double time_step_sec);
    void end_advance_timestep(double time_step_sec);
    void integrate(double time_step_sec) override;

    T _interactive_force_strength = static_cast<T>(1000);
    T _interactive_force_radius = static_cast<T>(50);
    VecType _interactive_force_position = VecType{};
    T _interactive_force_direction = static_cast<T>(1);
    
    std::vector<VecType> _new_positions;
    std::vector<VecType> _new_velocities;    
};


using ParticleSystemSolverd = ParticleSystemSolver<cato::Vec2d>;
using ParticleSystemSolverf = ParticleSystemSolver<cato::Vec2f>;
using ParticleSystemSolver3d = ParticleSystemSolver<cato::Vec3d>;
using ParticleSystemSolver3f = ParticleSystemSolver<cato::Vec3f>;

using ParticleSystemSolverPtrd = std::shared_ptr<ParticleSystemSolverd>;
using ParticleSystemSolverfPtr = std::shared_ptr<ParticleSystemSolverf>;
using ParticleSystemSolver3dPtr = std::shared_ptr<ParticleSystemSolver3d>;
using ParticleSystemSolver3fPtr = std::shared_ptr<ParticleSystemSolver3f>;


#endif