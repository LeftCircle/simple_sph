#include "shapes.h"


template <typename VecType>
void Shape<VecType>::bind_verts() {
    glGenVertexArrays(1, &this->_vao);
    glGenBuffers(1, &this->_vbo);
    glBindVertexArray(this->_vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->_vbo);
}


template <typename VecType>
VecType Box<VecType>::get_blc() const {
    return this->_center - (_dimensions / static_cast<typename VecType::value_type>(2));
}

template <typename VecType>
VecType Box<VecType>::get_trc() const {
    return this->_center + (_dimensions / static_cast<typename VecType::value_type>(2));
}

template <typename VecType>
void Box<VecType>::bind_verts() {
    Shape<VecType>::bind_verts();

    if constexpr (std::is_same<VecType, cato::Vec2T<typename VecType::value_type>>::value){
        // Do nothing. We are using legacy OpenGL immediate mode for 2D boxes.
    } else if constexpr (std::is_same<VecType, cato::Vec3T<typename VecType::value_type>>::value){
        // Bind 3D box vertices (not implemented)
        VecType halfDim = _dimensions / static_cast<typename VecType::value_type>(2);
        VecType vertices[8] = {
            this->_center + VecType{-halfDim.x, -halfDim.y, -halfDim.z},
            this->_center + VecType{ halfDim.x, -halfDim.y, -halfDim.z},
            this->_center + VecType{ halfDim.x,  halfDim.y, -halfDim.z},
            this->_center + VecType{-halfDim.x,  halfDim.y, -halfDim.z},
            this->_center + VecType{-halfDim.x, -halfDim.y,  halfDim.z},
            this->_center + VecType{ halfDim.x, -halfDim.y,  halfDim.z},
            this->_center + VecType{ halfDim.x,  halfDim.y,  halfDim.z},
            this->_center + VecType{-halfDim.x,  halfDim.y,  halfDim.z}
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(typename VecType::value_type), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    }
}

template <typename VecType>
void Box<VecType>::draw() const {
    // Placeholder implementation for drawing the box
    if constexpr (std::is_same<VecType, cato::Vec2T<typename VecType::value_type>>::value){
        // Draw 2D box
        VecType halfDim = _dimensions / static_cast<typename VecType::value_type>(2);
        VecType bottomLeft = this->_center - halfDim;
        glColor3f(1.0f, 0.0f, 0.0f);
        glBegin(GL_LINE_STRIP);
            glVertex2f(bottomLeft.x, bottomLeft.y);
            glVertex2f(bottomLeft.x + _dimensions.x, bottomLeft.y);
            glVertex2f(bottomLeft.x + _dimensions.x, bottomLeft.y + _dimensions.y);
            glVertex2f(bottomLeft.x, bottomLeft.y + _dimensions.y);
            glVertex2f(bottomLeft.x, bottomLeft.y);
        glEnd();
    } else if constexpr (std::is_same<VecType, cato::Vec3T<typename VecType::value_type>>::value){
        glDrawArrays(GL_LINE_LOOP, 0, 8);
    }
}

template <typename VecType>
void SolidBox<VecType>::bind_verts() {
    Shape<VecType>::bind_verts();
    if constexpr (std::is_same<VecType, cato::Vec2T<typename VecType::value_type>>::value){
        // Do nothing. We are using legacy OpenGL immediate mode for 2D boxes.
    } else if constexpr (std::is_same<VecType, cato::Vec3T<typename VecType::value_type>>::value){
        // Bind 3D solid box vertices
        // This would typically involve defining the vertices for all 6 faces
        VecType halfDim = this->get_dimensions() / static_cast<typename VecType::value_type>(2);
        VecType vertices[36] = {
            // Front face
            this->_center + VecType{-halfDim.x, -halfDim.y,  halfDim.z},
            this->_center + VecType{ halfDim.x, -halfDim.y,  halfDim.z},
            this->_center + VecType{ halfDim.x,  halfDim.y,  halfDim.z},
            this->_center + VecType{-halfDim.x, -halfDim.y,  halfDim.z},
            this->_center + VecType{ halfDim.x,  halfDim.y,  halfDim.z},
            this->_center + VecType{-halfDim.x,  halfDim.y,  halfDim.z},
            // Back face
            this->_center + VecType{-halfDim.x, -halfDim.y, -halfDim.z},
            this->_center + VecType{-halfDim.x,  halfDim.y, -halfDim.z},
            this->_center + VecType{ halfDim.x,  halfDim.y, -halfDim.z},
            this->_center + VecType{-halfDim.x, -halfDim.y, -halfDim.z},
            this->_center + VecType{ halfDim.x,  halfDim.y, -halfDim.z},
            this->_center + VecType{ halfDim.x, -halfDim.y, -halfDim.z},
            // Left face
            this->_center + VecType{-halfDim.x, -halfDim.y, -halfDim.z},
            this->_center + VecType{-halfDim.x, -halfDim.y,  halfDim.z},
            this->_center + VecType{-halfDim.x,  halfDim.y,  halfDim.z},
            this->_center + VecType{-halfDim.x, -halfDim.y, -halfDim.z},
            this->_center + VecType{-halfDim.x,  halfDim.y,  halfDim.z},
            this->_center + VecType{-halfDim.x,  halfDim.y, -halfDim.z},
            // Right face
            this->_center + VecType{ halfDim.x, -halfDim.y, -halfDim.z},
            this->_center + VecType{ halfDim.x,  halfDim.y, -halfDim.z},
            this->_center + VecType{ halfDim.x,  halfDim.y,  halfDim.z},
            this->_center + VecType{ halfDim.x, -halfDim.y, -halfDim.z},
            this->_center + VecType{ halfDim.x,  halfDim.y,  halfDim.z},
            this->_center + VecType{ halfDim.x, -halfDim.y,  halfDim.z},
            // Top face
            this->_center + VecType{-halfDim.x,  halfDim.y, -halfDim.z},
            this->_center + VecType{-halfDim.x,  halfDim.y, halfDim.z},
            this->_center + VecType{ halfDim.x,  halfDim.y, halfDim.z},
            this->_center + VecType{-halfDim.x,  halfDim.y, -halfDim.z},
            this->_center + VecType{ halfDim.x,  halfDim.y, halfDim.z},
            this->_center + VecType{ halfDim.x,  halfDim.y, -halfDim.z},
            // Bottom face
            this->_center + VecType{-halfDim.x, -halfDim.y, -halfDim.z},
            this->_center + VecType{ halfDim.x, -halfDim.y, -halfDim.z},
            this->_center + VecType{ halfDim.x, -halfDim.y,  halfDim.z},
            this->_center + VecType{-halfDim.x, -halfDim.y, -halfDim.z},
            this->_center + VecType{ halfDim.x, -halfDim.y,  halfDim.z},
            this->_center + VecType{-halfDim.x, -halfDim.y,  halfDim.z}
        };
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(typename VecType::value_type), (GLvoid*)0);
        glEnableVertexAttribArray(0);
    }
}

template <typename VecType>
void SolidBox<VecType>::draw() const {
    // Placeholder implementation for drawing the solid box
    if constexpr (std::is_same<VecType, cato::Vec2T<typename VecType::value_type>>::value){
        // Draw solid 2D box
    } else if constexpr (std::is_same<VecType, cato::Vec3T<typename VecType::value_type>>::value){
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}


template class Box<cato::Vec2T<float>>;
template class Box<cato::Vec3T<float>>;
template class Box<cato::Vec2T<double>>;
template class Box<cato::Vec3T<double>>;

template class SolidBox<cato::Vec2T<float>>;
template class SolidBox<cato::Vec3T<float>>;
template class SolidBox<cato::Vec2T<double>>;
template class SolidBox<cato::Vec3T<double>>;