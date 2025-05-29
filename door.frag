#version 330 core

in vec3 FragPos_world; // Zmieniona nazwa z FragPos
in vec3 Normal_world;  // Zmieniona nazwa z Normal
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 lightPos;   // Pozycja światła w przestrzeni świata
uniform vec4 lightColor;
uniform vec3 viewPos;    // Pozycja kamery w przestrzeni świata
uniform vec4 objectColor;
uniform sampler2D doorTexture;

void main()
{
    // Ambient lighting
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor.rgb;

    // Diffuse lighting
    vec3 norm = normalize(Normal_world); // Używamy Normal_world
    vec3 lightDir = normalize(lightPos - FragPos_world); // Używamy FragPos_world
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor.rgb;

    // Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos_world); // Używamy FragPos_world
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor.rgb;

    vec3 resultLight = (ambient + diffuse + specular);

    vec4 texColor = texture(doorTexture, TexCoord);
    // Użyj objectColor jeśli tekstura jest w pełni przezroczysta lub jeśli chcesz mieszać
    vec4 baseColor = texColor;
    if (texColor.a == 0.0) { // Możesz też użyć progu np. texColor.a < 0.1
        baseColor = objectColor;
    }
    // Lub prościej, jeśli tekstura drzwi zawsze powinna być widoczna:
    // vec4 baseColor = texture(doorTexture, TexCoord);

    FragColor = vec4(resultLight, 1.0) * baseColor;
}