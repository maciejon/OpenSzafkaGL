#version 330 core

in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D tex0;

// Camera and light
uniform vec3 camPos;
uniform vec3 lightDir = normalize(vec3(-0.3, -1.0, -0.3)); // Directional light
uniform vec3 lightColor = vec3(1.0);

// Material
uniform vec3 materialSpecular = vec3(0.5);
uniform float materialShininess = 32.0;

void main()
{
    // Normalize directions
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(camPos - fragPos);
    vec3 dirToLight = normalize(-lightDir); // Light comes *from* this direction

    // Top-fade factor: reduce brightness on top surfaces
    float facingUp = clamp(norm.y, 0.0, 1.0);   // 0 = side/down, 1 = top-facing
    float topFade = mix(1.0, 0.2, facingUp);    // full brightness to 20%

    // Ambient
    vec3 ambient = 0.1 * lightColor;

    // Diffuse
    float diff = max(dot(norm, dirToLight), 0.0);
    vec3 diffuse = diff * lightColor * topFade;

    // Specular
    vec3 reflectDir = reflect(-dirToLight, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), materialShininess);
    vec3 specular = spec * materialSpecular * topFade;

    // Final lighting
    vec3 lighting = ambient + diffuse + specular;
    // vec3 lighting = 10 * ambient;

    // Texture lookup
    vec4 texColor = texture(tex0, texCoords);

    // Final color — no vertex color multiplication
    vec3 finalColor = lighting * texColor.rgb;

    FragColor = vec4(finalColor, texColor.a);
}
