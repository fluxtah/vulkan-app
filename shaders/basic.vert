#version 450

layout(location = 0) in vec3 inPos;    // Position from vertex data
layout(location = 1) in vec3 inNormal; // Normal from vertex data
layout(location = 2) in vec2 inUV;     // UV coordinates from vertex data
layout(location = 3) in vec4 inTangent;     // Tangent coordinates from vertex data

layout(location = 0) out vec3 fragPos;  // Pass position to fragment shader
layout(location = 1) out vec3 normal;   // Pass normal to fragment shader
layout(location = 2) out vec2 uv;       // Pass UV coordinates to fragment shader
layout(location = 3) out vec4 tangent;       // Pass tangent coordinates to fragment shader

layout(set = 0, binding = 0) uniform UBO {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 lightDir;
    vec3 lightColor;
} ubo;

void main() {
    fragPos = vec3(ubo.model * vec4(inPos, 1.0));
    normal = mat3(transpose(inverse(ubo.model))) * inNormal; // Correct normal transformation
    uv = inUV; // Pass UV coordinates to the fragment shader
    tangent = inTangent;
    gl_Position = ubo.proj * ubo.view * vec4(fragPos, 1.0);
}
