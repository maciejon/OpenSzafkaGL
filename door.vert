#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal; // Dodajemy normalne
layout (location = 2) in vec2 aTexCoord; // Dodajemy współrzędne tekstury

out vec3 FragPos_world; // Pozycja fragmentu w przestrzeni świata
out vec3 Normal_world;  // Normalna w przestrzeni świata
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 camMatrix; // Zakładam, że to jest projection * view

void main()
{
    // Transformacja do przestrzeni świata
    FragPos_world = vec3(model * vec4(aPos, 1.0));
    // Normalne wymagają specjalnej transformacji (macierz normalnych)
    // aby poprawnie obsłużyć skalowanie niejednorodne.
    // Dla skalowania jednorodnego i rotacji/translacji, mat3(model) jest OK.
    // Pełna wersja: mat3(transpose(inverse(model))) * aNormal;
    Normal_world = mat3(model) * aNormal; // Uproszczona wersja, jeśli nie ma niejednorodnego skalowania
    // Normal_world = normalize(mat3(transpose(inverse(model))) * aNormal); // Pełniejsza wersja

    TexCoord = aTexCoord;

    gl_Position = camMatrix * vec4(FragPos_world, 1.0); // Używamy już przekształconego FragPos_world
}