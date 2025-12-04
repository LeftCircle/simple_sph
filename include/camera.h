#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>

#include "vector.h"
#include "matrix.h"

#define DEG2RAD(x) (x * 3.14159265359f / 180.0f)
#define RAD2DEG(x) (x * 180.0f / 3.14159265359f)
#define PI_OVER_2 1.57079632679f
#define PI 3.14159265359f

class Camera
{

public:
    Camera() {};
    ~Camera() {};

	//cato::Vec3f get_up_vector() { return cato::Vec3f(view_matrix[1], view_matrix[5], view_matrix[9]); };
	//cato::Vec3f get_tangent_vector() { return cato::Vec3f(view_matrix[0], view_matrix[4], view_matrix[8]); };

	void set_perspective_projection(float new_fov, float aspect, float znear, float zfar)
	{
		fov = new_fov;
		projection_matrix.perspective(DEG2RAD(new_fov), aspect, znear, zfar);
	}

	void lookat(cato::Vec3f& pos, cato::Vec3f& targ_v, cato::Vec3f& up_v)
	{
		position = pos, target = targ_v;
		view_matrix.set_view(pos, targ_v, up_v);
		_og_up = up_v;
	}

	//! Rotates the position of the camera by theta about the up vector and phi about the tangent vector
	//! while continuing to look at the target. 
	// void rotate(float t, float p)
	// {
	// 	cato::Vec3f old_up = get_up_vector();
	// 	cato::Vec3f old_tangent = get_tangent_vector();

	// 	Mat4f rot_p_general = Mat4f::Rotation(old_tangent, DEG2RAD(p));
	// 	Mat4f rot_t_general = Mat4f::Rotation(old_up, DEG2RAD(t));

	// 	//cato::Vec3f up = cato::Vec3f(rot_p_general * old_up);
	// 	cato::Vec3f up = cato::Vec3f(rot_t_general * rot_p_general * old_up);
	// 	position = cato::Vec3f(rot_t_general * rot_p_general * position);
	// 	view_matrix.SetView(position, target, up);
	// }
	// void rotate_about_og_up(float t, float p)
	// {
	// 	// Rotate the position and up vector about the original up vector
	// 	cato::Vec3f up = get_up_vector();
	// 	cato::Vec3f tangent = get_tangent_vector();

	// 	Mat4f rot_p_general = Mat4f::Rotation(tangent, DEG2RAD(p));
	// 	Mat4f rot_t_general = Mat4f::Rotation(_og_up, DEG2RAD(t));

	// 	position = cato::Vec3f(rot_t_general * rot_p_general * position);
	// 	up = cato::Vec3f(rot_t_general * rot_p_general * up);
	// 	view_matrix.SetView(position, target, up);
	// }

	// void rotate_other_position_about_og_up(cato::Vec3f& pos, float t, float p)
	// {
	// 	cato::Vec3f up = get_up_vector();
	// 	cato::Vec3f tangent = get_tangent_vector();

	// 	Mat4f rot_p_general = Mat4f::Rotation(tangent, DEG2RAD(p));
	// 	Mat4f rot_t_general = Mat4f::Rotation(_og_up, DEG2RAD(t));

	// 	pos = cato::Vec3f(rot_t_general * rot_p_general * position);
	// }
	
	// void zoom(float z)
	// {
	// 	cato::Vec3f direction = position - target;
	// 	direction.Normalize();
	// 	position += direction * z;
	// 	view_matrix.SetView(position, target, get_up_vector());
	// }


    cato::Vec3f position = cato::Vec3f(0, 0, 60);
	cato::Vec3f target = cato::Vec3f(0, 0, 0);
	Mat4f view_matrix;
	Mat4f projection_matrix;
	float fov = 35.0f;

private:
	cato::Vec3f _og_up = cato::Vec3f(0, 1, 0);
};

#endif