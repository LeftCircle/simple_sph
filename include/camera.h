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
    Camera() {
        set_perspective_projection(35.0f, 4.0f / 3.0f, 0.1f, 1000.0f);
        lookat(position, target, cato::Vec3f(0, 1, 0));
    };
    ~Camera() {};

	void set_perspective_projection(float new_fov, float aspect, float znear, float zfar)
	{
		fov = new_fov;
        float top = tan(fov * ONE_DEGREE_IN_RADIANS / 2.0f) * znear;
        float right = top * aspect;
        float left = -right;
        float bottom = -top;
        projection_mat.clear();
        projection_mat[0] = (2.0f * znear) / (right - left);
        projection_mat[5] = (2.0f * znear) / (top - bottom);
        projection_mat[10] = -(zfar + znear) / (zfar - znear);
        projection_mat[11] = -1.0f;
        projection_mat[14] = -(2.0f * zfar * znear) / (zfar - znear);
	}

	void lookat(const cato::Vec3f& pos, const cato::Vec3f& targ_v, const cato::Vec3f& up_v)
	{
		position = pos, target = targ_v;
		cato::Vec3f zaxis = (target - pos).normalized();
        cato::Vec3f xaxis = (up_v.cross(zaxis)).normalized();
        cato::Vec3f yaxis = zaxis.cross(xaxis);

        
        view_mat[0] = xaxis.x, view_mat[4] = xaxis.y, view_mat[8] =  xaxis.z, view_mat[12] = -pos.x;
        view_mat[1] = yaxis.x, view_mat[5] = yaxis.y, view_mat[9] =  yaxis.z, view_mat[13] = -pos.y;
        view_mat[2] = zaxis.x, view_mat[6] = zaxis.y, view_mat[10] = zaxis.z, view_mat[14] = -pos.z;
        view_mat[3] = 0.0f, view_mat[7] = 0.0f, view_mat[11] = 0.0f, view_mat[15] = 1.0f;

		_og_up = up_v;
	}

    
    cato::Vec3f position = cato::Vec3f(500.0, 0.0, 3000.0);
	cato::Vec3f target = cato::Vec3f(0.0, 0.0, 0.0);
	Mat4f view_mat;
	Mat4f projection_mat;
	float fov = 35.0f;

private:
	cato::Vec3f _og_up = cato::Vec3f(0, 1, 0);
};

#endif