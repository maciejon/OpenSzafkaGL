// Mesh.h
#pragma once

#include <glad/glad.h>
#include <vector>

class Mesh {
public:
    Mesh(const float* positions, const float* normals, const float* texCoords, const unsigned int* indices,
         int vertCount, int indexCount);
    ~Mesh();

    void Draw() const;

private:
    GLuint VAO, VBO, EBO;
    GLsizei indexCount;
};
