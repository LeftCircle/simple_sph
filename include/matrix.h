#ifndef MATRIX_H
#define MATRIX_H


#define DEG_TO_RAD(x) (x * 3.14159265359f / 180.0f)
#define RAD_TO_DEG(x) (x * 180.0f / 3.14159265359f)
#define ONE_DEGREE_IN_RADIANS 0.01745329252f

// Mat4CT - 4x4 column major matrix template
/* stored like this:
0 4 8  12
1 5 9  13
2 6 10 14
3 7 11 15*/
template<typename T>
struct Mat4CT {
    T m[16];

    Mat4CT() {
       for (int i = 0; i < 16; ++i) {
            m[i] = static_cast<T>(0);
        }
        m[0] = m[5] = m[10] = m[15] = static_cast<T>(1);
    }

    Mat4CT(const Mat4CT& other) {
        for (int i = 0; i < 16; ++i) {
            m[i] = other.m[i];
        }
    }

    Mat4CT(T val) {
        for (int i = 0; i < 16; ++i) {
            m[i] = val;
        }
    }

    static Mat4CT identity() {
        for (int i = 0; i < 16; ++i) {
            m[i] = static_cast<T>(0);
        }
        m[0] = m[5] = m[10] = m[15] = static_cast<T>(1);
        return *this;
    }

    void clear() {
        for (int i = 0; i < 16; ++i) {
            m[i] = static_cast<T>(0);
        }
    }

    // Access operator
    T& operator()(int row, int col) {
        return m[col * 4 + row];
    }

    T& operator[](int index) {
        return m[index];
    }

    // From https://github.com/LeftCircle/TeapotRender/blob/main/include/cyCodeBase/cyMatrix.h
    void set_view(const cato::Vec3T<T>& pos, const cato::Vec3T<T>& target, const cato::Vec3T<T>& up) {
        cato::Vec3T<T> zaxis = (target - pos).normalized();
        cato::Vec3T<T> xaxis = (up.cross(zaxis)).normalized();
        cato::Vec3T<T> yaxis = zaxis.cross(xaxis);

        m[0] = xaxis.x;    m[4] = yaxis.x;    m[8]  = -zaxis.x;   m[12] = static_cast<T>(0);
        m[1] = xaxis.y;    m[5] = yaxis.y;    m[9]  = -zaxis.y;   m[13] = static_cast<T>(0);
        m[2] = xaxis.z;    m[6] = yaxis.z;    m[10] = -zaxis.z;   m[14] = static_cast<T>(0);
        //m[3] = static_cast<T>(0);    m[7] = static_cast<T>(0);    m[11] = static_cast<T>(0);   m[15] = static_cast<T>(1);
        // For some reason CY codebase has the following:
        m[3] = -xaxis.dot(pos);    m[7] = -yaxis.dot(pos);    m[11] = -zaxis.dot(pos);   m[15] = static_cast<T>(1);
    }

    void perspective(T fov_radians, T aspect, T znear, T zfar) {
        float fov_rad = fovy * ONE_DEGREE_IN_RADIANS;
        float range = tan (fov_rad / 2.0f) * near;
        float sx = (2.0f * near) / (range * aspect + range * aspect);
        float sy = near / range;
        float sz = -(far + near) / (far - near);
        float pz = -(2.0f * far * near) / (far - near);
        clear();
        [0] = sx;
        [5] = sy;
        [10] = sz;
        [14] = pz;
        [11] = -1.0f;
    }

};

using Mat4f = Mat4CT<float>;
using Mat4d = Mat4CT<double>;


#endif