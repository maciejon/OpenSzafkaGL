#version 330 core

out vec4 FragColor;

in vec3 color;
in vec2 texCoord;

uniform sampler2D tex0;
uniform sampler2D tex1;

void main()
{
    float x = texCoord.x * 20;
    float y = texCoord.y * 20;

    if (sin(x)*sin(x) >= cos(x)*cos(y)*2)
        FragColor = texture(tex0, texCoord)*texCoord.x*pow((texCoord.y),3)*3;
    else
        FragColor = texture(tex1, texCoord);
}
