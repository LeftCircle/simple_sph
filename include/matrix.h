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

};

using Mat4f = Mat4CT<float>;
using Mat4d = Mat4CT<double>;


#endif