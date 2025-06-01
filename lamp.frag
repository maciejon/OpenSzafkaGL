#version 330 core

in vec3 fragPos;
in vec3 normal;
in vec3 color;
in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D tex0;

// Light and view parameters
uniform vec3 camPos;
uniform vec3 lightDir = normalize(vec3(-0.3, -1.0, -0.3)); // Directional light
uniform vec3 lightColor = vec3(1.0);

// Material properties
uniform vec3 materialSpecular = vec3(0.5);
uniform float materialShininess = 32.0;

void main()
{
    // Normalize
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(camPos - fragPos);
    vec3 dirToLight = normalize(-lightDir); // Directional light source

    // --- Lighting weights ---
    float facingUp = clamp(norm.y, 0.0, 1.0);   // 0 = side/down, 1 = up
    float topFade = mix(1.0, 0.2, facingUp);    // fade lighting on top

    // --- Lighting Components ---
    // Ambient (unchanged)
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

    // Apply texture and vertex color
    vec4 texColor = texture(tex0, texCoords);
    vec3 finalColor = lighting * texColor.rgb * color;

    FragColor = vec4(finalColor, texColor.a);
}
