#ifndef SPH_SYSTEM_DATA_H
#define SPH_SYSTEM_DATA_H

#include <vector>
#include <iostream>

#include "particle_system.h"
#include "vector.h"
#include "sph_kernel.h"

template<typename VecType>
class SPHSystemData : public ParticleSystem<VecType> {
public:
	using T = decltype(VecType().x);
	using Kernel = std::conditional_t<std::is_same<VecType, cato::Vec2T<T>>::value,
		SphSpikyKernal2<T>,
		SphSpikyKernal3<T>
	>;

	SPHSystemData();
	SPHSystemData(size_t n_particles);
	~SPHSystemData();

	void resize(const size_t n) override;

	const std::vector<T>& get_densities() const { return _densities; }
	std::vector<T>& get_densities() { return _densities; }

	const std::vector<T>& get_pressures() const { return _pressures; }
	std::vector<T>& get_pressures() { return _pressures; }

	// A helper function for udpating densities. Unlike the other interpolation 
	// functions, the density term cancels out in the SPH summation, so we have
	// a special helper function for it. That, and density is used to calculate
	// Every other interpolation, so it should be done first.
	void update_densities(const std::vector<VecType>& positions);

	T sum_of_nearby_kernal(const VecType& position) const;

	VecType interpolate(const VecType& position,
		const std::vector<VecType>& values) const;

	// Gradient is slightly different from interpolate, because we need to calculate
	// symmetric forces on each particle. We are also finding the gradient per particle, 
	// So we are passing in particle indices instead of positions. 
	VecType gradient_at(size_t particle_n, const std::vector<T>& values) const;

	// Minor tweak of subtracting value from origin particle results in constant values returning 
	// zero. 
	T laplacian_at(size_t particle_n, const std::vector<T>& values) const;
	
	T get_target_density() const { return _target_density; }
	void set_target_density(T d) { _target_density = d; }

private:
	std::vector<T> _densities;
	std::vector<T> _pressures;

	T _target_density = static_cast<T>(0.125);

};

using SPHSystemDatad = SPHSystemData<cato::Vec2d>;
using SPHSystemDataf = SPHSystemData<cato::Vec2f>;

using SPHSystemData3d = SPHSystemData<cato::Vec3d>;
using SPHSystemData3f = SPHSystemData<cato::Vec3f>;



#endif