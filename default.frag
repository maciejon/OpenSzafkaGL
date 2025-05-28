#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D tex0;
uniform sampler2D tex1;

void main()
{
    float x = texCoord.x * 20;
    float y = texCoord.y * 20;

        FragColor = texture(tex0, texCoord);
}
