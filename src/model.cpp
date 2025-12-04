#include "model.h"

Model* Model::pModel = nullptr;

Model::Model() {
    sph_system_solver = std::make_unique<SPHSystemSolver2d>();
    sph_visualization =  std::make_unique<SPHVisualization2D>();
}

Model::~Model() {}

Model* create_model() {
	Model* model = Model::instance();    
    return model;
}

void Model::simulate() {
    if (Controller::instance()->is_left_mouse_button_down()) {
        // Apply an interaction force at the mouse position
        cato::Vec2i mouse_pos = Controller::instance()->get_current_mouse_position();
        sph_system_solver->add_interaction_force(
            cato::Vec2d(static_cast<double>(mouse_pos.x), static_cast<double>(600 - mouse_pos.y)),
            200.0,
            1500.0
        );
        //std::cout << "Applying interaction force at (" << mouse_pos.x << ", " << (600 - mouse_pos.y) << ")\n";
    } else if (Controller::instance()->is_right_mouse_button_down()) {
        // Apply an interaction force at the mouse position
        cato::Vec2i mouse_pos = Controller::instance()->get_current_mouse_position();
        sph_system_solver->add_interaction_force(
            cato::Vec2d(static_cast<double>(mouse_pos.x), static_cast<double>(600 - mouse_pos.y)),
            200.0,
            1500.0,
            -1.0
        );
        //std::cout << "Applying interaction force at (" << mouse_pos.x << ", " << (600 - mouse_pos.y) << ")\n";
    } else {
        // No interaction force
        sph_system_solver->add_interaction_force(
            cato::Vec2d(0.0, 0.0),
            0.0,
            0.0
        );
    }
    
    sph_system_solver->update(0.05);
}

void Model::on_J_pressed() {

}

void Model::on_right_arrow_pressed() {
}

void Model::on_left_arrow_pressed() {
}

void Model::on_up_arrow_pressed() {
    // Increase the radius of the particles and then update the densities. 
    // print out the density of the special particle
    auto sph_data = sph_visualization->sph_data();
    double old_radius = sph_data->radius();
    double new_radius = old_radius * 1.1;
    sph_data->set_radius(new_radius);
    sph_data->build_neighbor_lookup(
        cato::Vec2i(static_cast<int>(800.0 / (new_radius * 2.0)) + 1,
					static_cast<int>(600.0 / (new_radius * 2.0)) + 1),
		new_radius * 2.0,
        sph_data->get_positions()
    );
    sph_data->find_each_neighbor();
    sph_data->update_densities(sph_data->get_positions());
    double density = sph_visualization->get_special_particle_density();
    std::cout << "Increased radius to " << new_radius << ", special particle density = " << density << "\n";
}

void Model::on_down_arrow_pressed() {
}

