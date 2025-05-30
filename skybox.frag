#version 330 core
out vec4 FragColor;

in vec3 TexCoords; // Współrzędne z vertex shadera

uniform samplerCube skybox; // Specjalny typ samplera dla cubemap

void main()
{
    FragColor = texture(skybox, TexCoords);
}