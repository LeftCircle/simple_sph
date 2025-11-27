#include "tests.h"


void test_boundary_conditions(){
    // Test that the new position and velocities are not updated if the particle is within bounds
    std::vector<cato::Vec2T<float>> new_positions = { {5.0f, 5.0f}, {15.0f, 15.0f} };
    std::vector<cato::Vec2T<float>> new_velocities = { {1.0f, 1.0f}, {1.0f, 1.0f} };
    const float x_min = 0.0f, x_max = 10.0f;
    const float y_min = 0.0f, y_max = 10.0f;
    const float restitution = 0.5f;
    CollisionHandler::apply_boundary_collisions(new_positions, new_velocities, x_min, x_max, y_min, y_max, restitution);
    assert(new_positions[0].x == 5.0f && new_positions[0].y == 5.0f);

    // confirm that the new positions of the second particle are within the bounds, and the magnitude of the velocity
    // is lower
    assert(new_positions[1].x < x_max && new_positions[1].y < y_max);
    assert(new_velocities[1].magnitude() < sqrt(2.0f));
    std::cout << "Boundary conditions test passed!" << std::endl;

}

void Tests::run_all_tests(){
    test_boundary_conditions();

    std::cout << "All tests passed!" << std::endl;
}