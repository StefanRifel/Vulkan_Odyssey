#version 450

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(push_constant) uniform PushConstantObject {
    vec2 offset;
} pco;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec3 fragNormal;
layout(location = 3) out vec3 fragPosition;

void main() {
    fragNormal = inNormal;
    fragTexCoord = inTexCoord;
    fragPosition = inPosition;
    gl_Position = ubo.proj * ubo.view * ubo.model * vec4(inPosition + vec3(pco.offset, 0.0), 1.0);
}