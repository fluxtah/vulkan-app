#version 450

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUV;
layout(location = 3) in vec4 inTangent;

layout(location = 4) in vec3 particlePos;  // World position of the particle (from compute shader)
layout(location = 5) in vec3 particleScale;  // Scale of the particle (from compute shader)

layout(location = 0) out vec3 fragPos;
layout(location = 1) out vec3 normal;
layout(location = 2) out vec2 uv;
layout(location = 3) out vec4 tangent;

layout(set = 0, binding = 0) uniform TransformUBO {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 cameraPos;
} ubo;

void main() {
    mat4 view = ubo.view;
    vec3 right = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 up = vec3(view[0][1], view[1][1], view[2][1]);

    mat4 billboardMatrix = mat4(
    vec4(right, 0.0),
    vec4(up, 0.0),
    vec4(cross(up, right), 0.0),
    vec4(0.0, 0.0, 0.0, 1.0)
    );

    // Scale the quad vertices
    vec4 scaledPos = vec4(inPos * particleScale, 1.0);

    // Use the billboard matrix and particle position directly
    vec4 worldPos = billboardMatrix * scaledPos + vec4(particlePos, 0.0);

    fragPos = vec3(worldPos); // Position in world space
    normal = mat3(transpose(inverse(billboardMatrix))) * inNormal;
    uv = inUV;
    tangent = inTangent;

    gl_Position = ubo.proj * ubo.view * worldPos; // Correct transformation to clip space
}


