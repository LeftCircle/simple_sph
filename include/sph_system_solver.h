#ifndef SPH_SYSTEM_SOLVER_H
#define SPH_SYSTEM_SOLVER_H

// This seperation of logic in Doyub Kim's fluid engine development book 
// seems really clean. The separation of the solver from the data helps
// keep the code very modular so it is easier to understand how each piece
// contributes.


#include "particle_system.h"
#include "vector.h"

template <typename T>
class SPHSystemSolver2T : public ParticleSystemSolver2D<T> {

public:
	SPHSystemSolver2T();
	virtual ~SPHSystemSolver2T();


protected:
	void accumulate_forces(double time_step_sec) override;


};




#endif