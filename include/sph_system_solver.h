#ifndef SPH_SYSTEM_SOLVER_H
#define SPH_SYSTEM_SOLVER_H

// This seperation of logic in Doyub Kim's fluid engine development book 
// seems really clean. The separation of the solver from the data helps
// keep the code very modular so it is easier to understand how each piece
// contributes.

#include <memory>
#include <vector>

#include "particle_system.h"
#include "vector.h"

template <typename T>
class SPHSystemSolver2T : public ParticleSystemSolver2D<T> {

public:
	SPHSystemSolver2T();
	SPHSystemSolver2T(size_t n_particles);
	virtual ~SPHSystemSolver2T();

	std::shared_ptr<SPHSystemData2<T>> sphSystemData() {
		return std::static_pointer_cast<SPHSystemData2<T>>(this->_particle_system);
	}


protected:
	void accumulate_forces(double time_step_sec) override;
	void on_begin_advance_timestep(double time_step_sec) override;
	void on_end_advance_timestep(double time_step_sec) override;

	virtual void accumulate_non_pressure_forces(double time_step_sec);
	virtual void accumulate_pressure_forces(double time_step_sec);
	virtual void accumulate_pressure_forces(std::vector<cato::Vec2T<T>>& positions,
		std::vector<T>& densities,
		std::vector<T>& pressures,
		std::vector<cato::Vec2T<T>>& forces);
	void accumulate_viscosity_forces();
	void compute_psuedo_viscosity();

	void compute_pressure();



};

using SPHSystemSolver2Td = SPHSystemSolver2T<double>;
using SPHSystemSolver2Tf = SPHSystemSolver2T<float>;


#endif