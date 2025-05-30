#version 330 core
layout (location = 0) in vec3 aPos;         // Pozycja wierzchołka
layout (location = 1) in vec3 aNormal;      // Normalna wierzchołka
layout (location = 2) in vec2 aTexCoord;    // Współrzędne tekstury

out vec3 FragPos_world; // Pozycja fragmentu w przestrzeni świata
out vec3 Normal_world;  // Normalna w przestrzeni świata
out vec2 TexCoord;      // Współrzędne tekstury (przekazane bez zmian)

uniform mat4 model;     // Macierz modelu (transformacja do przestrzeni świata)
uniform mat4 camMatrix; // Macierz kamery (view * projection)

void main()
{
    FragPos_world = vec3(model * vec4(aPos, 1.0));
    Normal_world = normalize(mat3(transpose(inverse(model))) * aNormal);
    TexCoord = aTexCoord;
    gl_Position = camMatrix * vec4(FragPos_world, 1.0);
}