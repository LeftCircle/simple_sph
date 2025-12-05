#ifndef SHAPES_H
#define SHAPES_H



#include <GL/glew.h>
#include <GL/glut.h>
#include <iostream>
#include "vector.h"

const int CIRCLE_SEGMENTS = 32;

template <typename VecType>
class Shape {
public:
    Shape() {}
    Shape(VecType center) : _center(center) {}
    virtual ~Shape() {}

    virtual void bind_verts();
    virtual void draw() const = 0;
    void bind_vao() const { glBindVertexArray(this->_vao); }
    const VecType& get_center() const { return _center; }

protected:
    VecType _center;
    GLuint _vao;
    GLuint _vbo;
};


template <typename VecType>
class Box : public Shape<VecType> {
public:
    Box() {};
    Box(VecType center, VecType dimensions)
        : Shape<VecType>(center), _dimensions(dimensions) {}
    
    ~Box() {};

    VecType get_blc() const;
    VecType get_trc() const;

    const VecType& get_dimensions() const { return _dimensions; }
    void set_dimensions(const VecType& dimensions) { _dimensions = dimensions; }

    
    virtual void draw() const override;
    virtual void bind_verts() override;

private:
    VecType _dimensions;
};

template <typename VecType>
class SolidBox : public Box<VecType> {
public:
    SolidBox() {};
    SolidBox(VecType center, VecType dimensions)
        : Box<VecType>(center, dimensions) {}

    ~SolidBox() {};

    virtual void draw() const override;
    virtual void bind_verts() override;

};


#ifndef CUBE_H
#define CUBE_H

class LucasCube {
    
    private:
        unsigned int cubeVBO, cubeVAO; // Declare buffers
    
    public:
        LucasCube() {

            // Vertex/Index data
            // -----------------
            float cubeVertices[] = {
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
                -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
                -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
                -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

                0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
                0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
                0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
                0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
                0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
                -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
                0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
                -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
            };

            // Bind and load buffers with vertex/index data for cube
            // -----------------------------------------------------
            glGenBuffers(1, &cubeVBO);		// Generate IDs for buffers
            glGenVertexArrays(1, &cubeVAO);

            glBindVertexArray(cubeVAO); // Bind the vertex array first

            glBindBuffer(GL_ARRAY_BUFFER, cubeVBO); // Bind as an array buffer for vertices
            glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW); // Load vertex data into VBO

            // Position data
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // Locate the vertex position attribute on GPU
            glEnableVertexAttribArray(0);

            // Normal data
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            
        }

        void draw() {
            glDrawArrays(GL_TRIANGLES, 0, 36); // Draw the 36 vertices (1 cube, 6 faces, 12 triangles)
        }

        void draw_outline() {
            glDrawArrays(GL_LINE_LOOP, 0, 36); // Draw the 36 vertices (1 cube, 6 faces, 12 triangles)
        }

        void bind_vao() {
            glBindVertexArray(cubeVAO);
        }

};

#endif


using Box2D = Box<cato::Vec2T<float>>;
using Box3D = Box<cato::Vec3T<float>>;
using Box2Dd = Box<cato::Vec2T<double>>;
using Box3Dd = Box<cato::Vec3T<double>>;

using SolidBox2D = SolidBox<cato::Vec2T<float>>;
using SolidBox3D = SolidBox<cato::Vec3T<float>>;



#endif