#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices,
           std::vector<GLuint>& indices,
           std::vector<Texture>& textures)
    : vertices(vertices), indices(indices), textures(textures)
{
    VAO.Bind();

    VBO_ptr = new VBO(reinterpret_cast<GLfloat*>(vertices.data()), vertices.size() * sizeof(Vertex));
    EBO_ptr = new EBO(indices.data(), indices.size() * sizeof(GLuint));

    VAO.LinkAttrib(*VBO_ptr, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0);                      // Position
    VAO.LinkAttrib(*VBO_ptr, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3 * sizeof(float)));    // Normal
    VAO.LinkAttrib(*VBO_ptr, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6 * sizeof(float)));    // Color
    VAO.LinkAttrib(*VBO_ptr, 3, 2, GL_FLOAT, sizeof(Vertex), (void*)(9 * sizeof(float)));    // TexCoords

    VAO.Unbind();
    VBO_ptr->Unbind();
    EBO_ptr->Unbind();
}


void Mesh::Draw(Shader& shader, Camera& camera, const glm::mat4& model)
{
    shader.Activate();
    VAO.Bind();

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        // Use generic names like tex0, tex1, tex2...
        std::string uniformName = "tex" + std::to_string(i);
        textures[i].texUnit(shader, uniformName.c_str(), i);
        textures[i].Bind();
    }

    // glUniform3fv(glGetUniformLocation(shader.ID, "lightPos"), 1, glm::value_ptr(lightPosVal));
    // glUniform4fv(glGetUniformLocation(shader.ID, "lightColor"), 1, glm::value_ptr(lightColorVal));
    // glUniform3fv(glGetUniformLocation(shader.ID, "viewPos"), 1, glm::value_ptr(camera.Position));
    // glUniform4f(glGetUniformLocation(shader.ID, "objectColor"), 1.0f, 1.0f, 1.0f, 1.0f);

    glUniform3f(glGetUniformLocation(shader.ID, "camPos"),
                camera.Position.x, camera.Position.y, camera.Position.z);
    camera.Matrix(shader, "camMatrix");
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
}

Mesh::~Mesh()
{
    VAO.Delete();
    if (VBO_ptr) { VBO_ptr->Delete(); delete VBO_ptr; }
    if (EBO_ptr) { EBO_ptr->Delete(); delete EBO_ptr; }
}
