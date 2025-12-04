#ifndef SPH_SYSTEM_SOLVER_H
#define SPH_SYSTEM_SOLVER_H

// This seperation of logic in Doyub Kim's fluid engine development book 
// seems really clean. The separation of the solver from the data helps
// keep the code very modular so it is easier to understand how each piece
// contributes.

#include <memory>
#include <vector>
#include <iostream>

#include "sph_kernel.h"
#include "sph_system_data2.h"
#include "vector.h"
#include "particle_system_solver.h"


template <typename T>
class SPHSystemSolver2T : public ParticleSystemSolver<cato::Vec2T<T>> {
public:
	SPHSystemSolver2T();
	SPHSystemSolver2T(size_t n_particles);
	virtual ~SPHSystemSolver2T();

	std::shared_ptr<SPHSystemData2<T>> sphSystemData() {
		return std::static_pointer_cast<SPHSystemData2<T>>(this->_particle_system);
	}

	T get_viscosity_coefficient() const { return _viscosity_coefficient; }
	void set_viscosity_coefficient(T v) { _viscosity_coefficient = v; }
	T get_eos_exponent() const { return _eos_exponent; }
	void set_eos_exponent(T e) { _eos_exponent = e; }
	T get_pseudo_viscosity_coefficient() const { return _pseudoViscosityCoefficient; }
	void set_pseudo_viscosity_coefficient(T v) { _pseudoViscosityCoefficient = v; }
	T get_speed_of_sound() const { return speed_of_sound; }
	void set_speed_of_sound(T s) { speed_of_sound = s; }
	T get_negative_pressure_scale() const { return negative_pressure_scale; }
	void set_negative_pressure_scale(T s) { negative_pressure_scale = s; }

	void update_graphics() override;


protected:
	void on_update(double delta) override;
	void accumulate_forces() override;
	void on_begin_advance_timestep(double time_step_sec) override;
	void on_end_advance_timestep(double time_step_sec) override;

	virtual void accumulate_non_pressure_forces();
	virtual void accumulate_pressure_forces();
	virtual void accumulate_pressure_forces(
		const std::vector<cato::Vec2T<T>>& positions,
		const std::vector<T>& densities,
		const std::vector<T>& pressures,
		std::vector<cato::Vec2T<T>>& forces);
	void accumulate_viscosity_forces();
	void compute_psuedo_viscosity(double time_step_sec);

	void compute_pressure();
	double compute_pressure_from_eos(
		T density,
		T target_desnsity,
		T eos_scale,
		T eos_exponent,
		T negative_pressure_scale
	);

	T _viscosity_coefficient = static_cast<T>(0.1);
	T _eos_exponent = static_cast<T>(3.0);
	T _pseudoViscosityCoefficient = static_cast<T>(0.5);
	T speed_of_sound = static_cast<T>(175.0);
	T negative_pressure_scale = static_cast<T>(0.0);
	std::vector<cato::Vec2T<T>> _predicted_positions;
	std::vector<cato::Vec2T<T>> _predicted_velocities;

};

using SPHSystemSolver2d = SPHSystemSolver2T<double>;
using SPHSystemSolver2f = SPHSystemSolver2T<float>;


#endif