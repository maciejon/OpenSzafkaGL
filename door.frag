#version 330 core

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec4 lightColor;
uniform vec3 viewPos;
uniform vec4 objectColor; // Can be used if no texture
uniform sampler2D doorTexture; // Optional texture

void main()
{
    // Ambient lighting
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor.rgb;

    // Diffuse lighting
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor.rgb;

    // Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor.rgb;

    vec3 resultLight = (ambient + diffuse + specular);

    vec4 texColor = texture(doorTexture, TexCoord);
    vec4 baseColor = texColor.a > 0.0 ? texColor : objectColor;

    FragColor = vec4(resultLight, 1.0) * baseColor;
}
