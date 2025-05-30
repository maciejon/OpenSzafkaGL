#version 330 core
layout (location = 0) in vec3 aPos;      
layout (location = 1) in vec3 aNormal;      
layout (location = 2) in vec2 aTexCoord;  

out vec3 FragPos_world; 
out vec3 Normal_world;
out vec2 TexCoord;     

uniform mat4 model;    
uniform mat4 camMatrix; 

void main()
{
    FragPos_world = vec3(model * vec4(aPos, 1.0));
    Normal_world = normalize(mat3(transpose(inverse(model))) * aNormal);
    TexCoord = aTexCoord;
    gl_Position = camMatrix * vec4(FragPos_world, 1.0);
}