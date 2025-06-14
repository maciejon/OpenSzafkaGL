#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoords;

out vec3 fragPos;
out vec3 normal;
out vec3 color;
out vec2 texCoords;

uniform mat4 model;
uniform mat4 camMatrix;

void main()
{
    fragPos = vec3(model * vec4(aPos, 1.0));
    normal = mat3(transpose(inverse(model))) * aNormal;
    color = aColor;
    texCoords = aTexCoords;

    gl_Position = camMatrix * vec4(fragPos, 1.0);
}
