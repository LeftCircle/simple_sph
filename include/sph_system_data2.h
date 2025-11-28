#ifndef SPH_SYSTEM_DATA2_H
#define SPH_SYSTEM_DATA2_H

#include <vector>

#include "particle_system.h"
#include "vector.h"
#include "sph_kernel.h"

template<typename T>
class SPHSystemData2 : public ParticleSystem2D<T> {
public:
	SPHSystemData2();
	~SPHSystemData2();

	const std::vector<T>& get_densities() const { return _densities; }
	std::vector<T>& get_densities() { return _densities; }

	// A helper function for udpating densities. Unlike the other interpolation 
	// functions, the density term cancels out in the SPH summation, so we have
	// a special helper function for it. That, and density is used to calculate
	// Every other interpolation, so it should be done first.
	void update_densities();

	T sum_of_nearby_kernal(const cato::Vec2T<T>& position) const;

	cato::Vec2T<T> interpolate(const cato::Vec2T<T>& position,
		const std::vector<cato::Vec2T<T>>& values) const;

	// Gradient is slightly different from interpolate, because we need to calculate
	// symmetric forces on each particle. We are also finding the gradient per particle, 
	// So we are passing in particle indices instead of positions. 
	cato::Vec2T<T> gradient_at(size_t particle_n, const std::vector<cato::Vec2T<T>>& values) const;

	// Minor tweak of subtracting value from origin particle results in constant values returning 
	// zero. 
	double laplacian_at(size_t particle_n, const std::vector<cato::Vec2T<T>>& values) const;

private:
	std::vector<T> _densities;

};

using SPHSystemData2d = SPHSystemData2<double>;
using SPHSystemData2f = SPHSystemData2<float>;


#endif