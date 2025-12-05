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
		std::cout << "Usage: " << argv[0] << " <num_particles>\n";
		return 1;
	}
	int num_particles = std::atoi(argv[1]);

	
	View* view = create_view();
	Controller* controller = create_controller();
	
	Model* model = create_model();
	
	model->sph_system_solver->sphSystemData()->set_radius(50.0);
	model->sph_system_solver->sphSystemData()->resize(num_particles);
	//model->sph_system_solver->sphSystemData()->randomize_particles(50.0, 750.0, 50.0, 550.0);
	//model->sph_system_solver->sphSystemData()->set_particle_position(0, cato::Vec2d(400.0, 300.0));
	model->sph_system_solver->sphSystemData()->organize_particles_in_grid(
		cato::Vec2i(400, 300),
		cato::Vec2i(10, 10),
		cato::Vec2i(30, 30)
	);

	// model->sph_visualization->organize_particles_in_grid(
	// 	cato::Vec2i(400, 300),
	// 	cato::Vec2i(10, 10),
	// 	cato::Vec2i(100, 100)
	// );
	// model->sph_visualization->sph_data()->set_radius(50.0);


	view->init(argc, argv, WINDOW_WIDTH, WINDOW_HEIGHT);
	view->main_loop();


	return 0;

}