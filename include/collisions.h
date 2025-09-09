#ifndef COLLISIONS_H
#define COLLISIONS_H


#include <vector>
#include <cstddef>

#include "vector.h"


class CollisionHandler{
public:
	template<typename T>
	static void apply_boundary_collisions(std::vector<T>& new_positions,
		std::vector<T>& new_velocities,
		const float x_min, const float x_max,
		const float y_min, const float y_max,
		const float restitution = 0.5f)
	{
		for (std::size_t i = 0; i < new_positions.size(); i++) {
			// Check X boundaries
			if (new_positions[i].x < x_min) {
				new_positions[i].x = x_min;
				new_velocities[i].x = -new_velocities[i].x * restitution;
			}
			else if (new_positions[i].x > x_max) {
				new_positions[i].x = x_max;
				new_velocities[i].x = -new_velocities[i].x * restitution;
			}
			// Check Y boundaries
			if (new_positions[i].y < y_min) {
				new_positions[i].y = y_min;
				new_velocities[i].y = -new_velocities[i].y * restitution;
			}
			else if (new_positions[i].y > y_max) {
				new_positions[i].y = y_max;
				new_velocities[i].y = -new_velocities[i].y * restitution;
			}
		}
	}
};

#endif // COLLISIONS_H