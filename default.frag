#version 330 core

out vec4 FragColor;

in vec2 texCoord;

uniform sampler2D woodTexture;
uniform sampler2D concreteTexture;
uniform sampler2D grassTexture;
uniform sampler2D tex0;

uniform int materialType; // 0 for wood, 1 for concrete, 2 for grass

void main()
{
    if (materialType == 0) {
        FragColor = texture(woodTexture, texCoord);
    } else if (materialType == 1) {
        FragColor = texture(concreteTexture, texCoord);
    } else if (materialType == 2) {
        FragColor = texture(grassTexture, texCoord);
    } else {
        FragColor = vec4(1.0, 0.0, 1.0, 1.0); // Debug: Magenta for untextured
    }
}