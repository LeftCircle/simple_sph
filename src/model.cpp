#include "model.h"

Model* Model::pModel = nullptr;

Model::Model() {
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
}

void Model::on_down_arrow_pressed() {
}

