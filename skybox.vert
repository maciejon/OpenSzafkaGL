#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords; // Współrzędne tekstury dla cubemapy

uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aPos; // Używamy pozycji wierzchołków jako współrzędnych tekstury dla cubemapy
    // Usuwamy komponent translacji z macierzy widoku, aby skybox podążał za kamerą
    mat4 viewNoTranslation = mat4(mat3(view));
    vec4 pos = projection * viewNoTranslation * vec4(aPos, 1.0);
    // Ustawiamy z na w, aby głębokość była zawsze 1.0 (najdalszy obiekt)
    // To zapewnia, że skybox jest rysowany za wszystkim innym.
    gl_Position = pos.xyww;
}