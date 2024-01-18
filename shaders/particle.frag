#version 450

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 tangent;

layout(location = 0) out vec4 outColor;

layout(set = 1, binding = 0) uniform sampler2D texSampler;

void main() {
    vec4 textureColor = texture(texSampler, uv);

    outColor = vec4(textureColor.rgb * 2.0, textureColor.a);
}
