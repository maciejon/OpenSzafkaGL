#version 330 core

in vec3 FragPos_world;
in vec3 Normal_world;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 lightPos;
uniform vec4 lightColor;
uniform vec3 viewPos;
uniform vec4 objectColor;
uniform sampler2D tex0;

void main()
{
    vec3 lightingResult = vec3(0.0);
    // Ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor.rgb;
    // Diffuse
    vec3 norm = normalize(Normal_world);
    vec3 lightDir = normalize(lightPos - FragPos_world);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor.rgb;
    // Specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos_world);
    vec3 reflectDir = reflect(-lightDir, norm);
    float shininess = 32.0;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = specularStrength * spec * lightColor.rgb;

    lightingResult = ambient + diffuse + specular;

    // === Texture or fallback ===
    vec4 texColor = useTexture ? texture(tex0, TexCoord) : vec4(1.0);
    vec4 textureColor = texture(tex0, TexCoord);
    FragColor = vec4(lightingResult, 1.0) * textureColor;
}