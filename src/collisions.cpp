#include "collisions.h"


// Template function definition
// TO DO -> find the time of the collision and correct positions and velocities accordingly
// Handle the corner case where a particle could collide off an edge near the corner and be
// corrected out of bounds. This could be handled by iterating until no collisions are detected
// template<typename T>
// void apply_boundary_collisions(std::vector<T>& new_positions,
// 	std::vector<T>& new_velocities,
// 	const std::vector<T>& old_positions,
// 	const std::vector<T>& old_velocities,
// 	const float dt,
// 	const float x_min, const float x_max,
// 	const float y_min, const float y_max,
// 	const float restitution){
// 	for (std::size_t i = 0; i < new_positions.size(); i++) {
// 		// Check X boundaries
// 		if (new_positions[i].x < x_min) {
// 			new_positions[i].x = x_min;
// 			new_velocities[i].x = -new_velocities[i].x * restitution;
// 		}
// 		else if (new_positions[i].x > x_max) {
// 			new_positions[i].x = x_max;
// 			new_velocities[i].x = -new_velocities[i].x * restitution;
// 		}
// 		if (new_positions[i].y < y_min) {
// 			new_positions[i].y = y_min;
// 			new_velocities[i].y = -new_velocities[i].y * restitution;
// 		}
// 		else if (new_positions[i].y > y_max) {
// 			new_positions[i].y = y_max;
// 			new_velocities[i].y = -new_velocities[i].y * restitution;
// 		}
// 	}
// }