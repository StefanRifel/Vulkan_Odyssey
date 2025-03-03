#version 450

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragPosition;

layout(location = 0) out vec4 outColor;

const vec3 lightPos = vec3(-4.0f, 14.0f, 12.0f);  // Position des Lichts
const vec3 lightColor = vec3(1.0, 1.0, 1.0); // Weißes Licht
const vec3 viewPos = vec3(0.0, 0.0, 5.0);    // Kamera weiter hinten

void main() {
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPosition);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 ambient = 0.1 * lightColor;

    vec3 viewDir = normalize(-fragPosition);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = spec * lightColor;

    vec3 textureColor = texture(texSampler, fragTexCoord).rgb;

    vec3 finalColor = (ambient + diffuse + specular) * textureColor;
    outColor = vec4(finalColor, 1.0);
}