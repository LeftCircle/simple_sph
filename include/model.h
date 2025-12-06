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

    static Model* instance(bool use3D = false) {
		if(pModel==nullptr)
		{
			pModel = new Model(use3D);
		}
		return pModel;
	}
    
	~Model();
	
	void on_J_pressed();
	void on_up_arrow_pressed();
	void on_down_arrow_pressed();
	void on_right_arrow_pressed();
	void on_left_arrow_pressed();

    std::unique_ptr<SPHSystemSolver2d> sph_system_solver2D;
    std::unique_ptr<SPHSystemSolver3d> sph_system_solver3D;
	//std::unique_ptr<SPHVisualization2D> sph_visualization;

	void simulate();
	bool is_using_3D() const { return use3D; }	
	
private:
	
	bool use3D = false;
	static Model* pModel;
	State _current_state = DEFAULT;

	Model(bool use3D);
	Model() = delete;
	Model( const Model& );
	Model& operator= (const Model& );

};


Model* create_model(bool use3D);

#endif