#ifndef MODEL_H
#define MODEL_H

#include <memory>
#include <iostream>

#include "controller.h"
#include "vector.h"
#include "sph_system_solver.h"
#include "sph_visualization.h"
#include "shapes.h"
#include "sph_system_data.h"

class Model
{
public:
	enum State {
		DEFAULT,
		JULIA_SET
	};

    static Model* instance() {
		if(pModel==nullptr)
		{
			pModel = new Model();
		}
		return pModel;
	}
    
	~Model();
	
	void on_J_pressed();
	void on_up_arrow_pressed();
	void on_down_arrow_pressed();
	void on_right_arrow_pressed();
	void on_left_arrow_pressed();

    //std::unique_ptr<SPHSystemSolver2d> sph_system_solver;
    std::unique_ptr<SPHSystemSolver2d> sph_system_solver;
	std::unique_ptr<SPHVisualization2D> sph_visualization;

	void simulate();


private:

	static Model* pModel;
	State _current_state = DEFAULT;

	Model();
	Model( const Model& );
	Model& operator= (const Model& );

};


Model* create_model();

#endif