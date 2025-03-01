#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 texCoord;  // Nur eine Output-Variable

void main() {
    texCoord = inPosition;  // Position direkt als Texturkoordinate verwenden
    
    // Entferne Translation aus View-Matrix durch Konvertierung zu mat3
    vec4 pos = ubo.proj * mat4(mat3(ubo.view)) * ubo.model * vec4(inPosition, 1.0);
    gl_Position = pos.xyww;  // Force depth to furthest plane
}