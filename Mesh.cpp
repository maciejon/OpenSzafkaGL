// Mesh.cpp
#include "Mesh.h"

Mesh::Mesh(const float* positions, const float* normals, const float* texCoords, const unsigned int* indices,
           int vertCount, int indexCount)
{
    indexCount = indexCount;

    std::vector<float> vertexData;
    vertexData.reserve(vertCount * 8); // 3 pos + 3 normal + 2 tex

    for (int i = 0; i < vertCount; ++i) {
        vertexData.push_back(positions[i * 3 + 0]);
        vertexData.push_back(positions[i * 3 + 1]);
        vertexData.push_back(positions[i * 3 + 2]);

        vertexData.push_back(normals[i * 3 + 0]);
        vertexData.push_back(normals[i * 3 + 1]);
        vertexData.push_back(normals[i * 3 + 2]);

        vertexData.push_back(texCoords[i * 2 + 0]);
        vertexData.push_back(texCoords[i * 2 + 1]);
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(float), vertexData.data(), GL_STATIC_DRAW);

    // index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // texCoord
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Mesh::Draw() const {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
