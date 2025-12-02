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
        static_cast<int>(800.0 / (new_radius * 2.0)) + 1,
        static_cast<int>(600.0 / (new_radius * 2.0)) + 1,
        new_radius * 2.0
    );
    sph_data->find_each_neighbor();
    sph_data->update_densities();
    double density = sph_visualization->get_special_particle_density();
    std::cout << "Increased radius to " << new_radius << ", special particle density = " << density << "\n";
}

void Model::on_down_arrow_pressed() {
}

