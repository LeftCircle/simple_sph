#ifndef COLLISIONS_H
#define COLLISIONS_H


#include <vector>
#include <cstddef>
#include <iostream>

#include "vector.h"

const float DEFAULT_OFFSET = 0.01f;

class CollisionHandler{
public:
	template<typename VecType>
	static void apply_boundary_collisions(std::vector<VecType>& new_positions,
		std::vector<VecType>& new_velocities,
		const VecType& blc,
		const VecType& trc,
		const float restitution = 0.5f)
	{
		for (std::size_t i = 0; i < new_positions.size(); i++) {
			// Check X boundaries
			if (new_positions[i].x < blc.x) {
				
				new_positions[i].x = blc.x + DEFAULT_OFFSET;
				new_velocities[i].x = -new_velocities[i].x * restitution;
			}
			else if (new_positions[i].x > trc.x) {
				new_positions[i].x = trc.x - DEFAULT_OFFSET;
				new_velocities[i].x = -new_velocities[i].x * restitution;
			}
			// Check Y boundaries
			if (new_positions[i].y < blc.y) {
				new_positions[i].y = blc.y + DEFAULT_OFFSET;
				new_velocities[i].y = -new_velocities[i].y * restitution;
			}
			else if (new_positions[i].y > trc.y) {
				new_positions[i].y = trc.y - DEFAULT_OFFSET;
				new_velocities[i].y = -new_velocities[i].y * restitution;
			}
			// Check Z boundaries for 3D vectors
			if constexpr (std::is_same<VecType, cato::Vec3T<typename VecType::value_type>>::value) {
				if (new_positions[i].z < blc.z) {
					new_positions[i].z = blc.z + DEFAULT_OFFSET;
					new_velocities[i].z = -new_velocities[i].z * restitution;
				}
				else if (new_positions[i].z > trc.z) {
					new_positions[i].z = trc.z - DEFAULT_OFFSET;
					new_velocities[i].z = -new_velocities[i].z * restitution;
				}
			}
		}
	}
};

#endif // COLLISIONS_H