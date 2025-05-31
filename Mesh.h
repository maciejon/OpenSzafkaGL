#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "camera.h"
#include "Texture.h"
#include "shaderClass.h"

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texUV;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    VAO VAO;

    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);
    // void Draw(Shader& shader, Camera& camera);
    void Draw(Shader& shader, Camera& camera, const glm::mat4& model);
    ~Mesh(); 

private:
    VBO* VBO_ptr;
    EBO* EBO_ptr;
};

#endif
