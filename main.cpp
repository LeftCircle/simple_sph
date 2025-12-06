#include <iostream>
#include <cstdlib>

#include "command_line_parser.h"
#include "tests.h"
#include "particle_system_solver.h"
#include "sph_system_solver.h"

#include "model.h"
#include "view.h"
#include "controller.h"

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;


int main(int argc, char** argv) {
	CommandLineParser cmd_parser(argc, argv);
	if (cmd_parser.has_flag("-t")){
		Tests::run_all_tests();
		return 0;
	}

	if (argc < 2) {
		std::cout << "Usage: " << argv[0] << " <3> for 3D. Anything else means 2D\n";
		return 1;
	}
	bool use3D = std::atoi(argv[1]) == 3;

	
	View* view = create_view();
	Controller* controller = create_controller();
	
	Model* model = create_model(use3D);
	
	model->sph_system_solver2D->sphSystemData()->set_radius(50.0);
	model->sph_system_solver3D->sphSystemData()->set_radius(20.0);
	
	
	// For 3D
	// model->sph_system_solver->sphSystemData()->organize_particles_in_grid(
    //     cato::Vec3i(0, 0, 0),
    //     cato::Vec3i(10, 10, 10),
    //     cato::Vec3i(30, 30, 30)
    // );

	// For 2D
	if (!use3D) {
		model->sph_system_solver2D->sphSystemData()->organize_particles_in_grid(
			cato::Vec2i(400, 300),
			cato::Vec2i(10, 10),
			cato::Vec2i(30, 30)
		);
	} else{
		model->sph_system_solver3D->sphSystemData()->organize_particles_in_grid(
			cato::Vec3i(0, 0, 0),
			cato::Vec3i(10, 10, 10),
			cato::Vec3i(30, 30, 30)
		);
	}

	view->init(argc, argv, WINDOW_WIDTH, WINDOW_HEIGHT);

	model->sph_system_solver2D->bind_boundary_box_verts();
	model->sph_system_solver3D->bind_boundary_box_verts();
	model->sph_system_solver2D->particleSystem()->bind_particle_model_verts();
	model->sph_system_solver3D->particleSystem()->bind_particle_model_verts();


	// model->sph_visualization->organize_particles_in_grid(
	// 	cato::Vec2i(400, 300),
	// 	cato::Vec2i(10, 10),
	// 	cato::Vec2i(100, 100)
	// );
	// model->sph_visualization->sph_data()->set_radius(50.0);


	view->main_loop();


	return 0;

}