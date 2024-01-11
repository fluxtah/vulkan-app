#version 450

layout(location = 0) in vec3 inPosition;

layout(push_constant) uniform PushConstants {
    mat4 model;
    mat4 view;
    mat4 proj;
} pushConstants;

void main() {
    gl_Position = pushConstants.proj * pushConstants.view * pushConstants.model * vec4(inPosition, 1.0);
}
