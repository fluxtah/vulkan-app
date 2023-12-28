#version 450

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 tangent;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform UBO {
    mat4 model;
    mat4 view;
    mat4 proj;
    vec3 lightDir;
    vec4 lightColor;
} lightInfo;

layout(set = 0, binding = 1) uniform sampler2D texSampler;
layout(set = 0, binding = 2) uniform sampler2D normalMapSampler;
layout(set = 0, binding = 3) uniform sampler2D metallicRoughnessMapSampler;

vec3 calculateLight(vec3 norm, float metallic, float roughness) {
    vec3 lightPos = vec3(-4, 20, 20);
    vec3 toLight = normalize(lightPos - fragPos);
    vec3 viewDir = normalize(-fragPos);
    vec3 halfDir = normalize(toLight + viewDir);

    // Basic ambient component
    vec3 ambient = 0.2 * lightInfo.lightColor.rgb;

    // Diffuse reflection (Lambert)
    float diff = max(dot(norm, toLight), 0.0);
    vec3 diffuse = (1.0 - metallic) * diff * lightInfo.lightColor.rgb;

    // Specular reflection (Blinn-Phong model modified by roughness)
    float specularStrength = pow(max(dot(norm, halfDir), 0.0), mix(8.0, 256.0, 1.0 - roughness));
    vec3 specularColor = mix(vec3(0.04), lightInfo.lightColor.rgb, metallic);
    vec3 specular = specularColor * specularStrength;

    return ambient + diffuse + specular;
}

void main() {
    vec3 bitangent = cross(normal, tangent.xyz) * tangent.w;
    mat3 TBN = mat3(normalize(tangent.xyz), normalize(bitangent), normalize(normal));
    vec3 sampledNormal = texture(normalMapSampler, uv).rgb * 2.0 - 1.0;
    vec3 norm = normalize(TBN * sampledNormal);

    vec3 metallicRoughness = texture(metallicRoughnessMapSampler, uv).rgb;
    float metallic = metallicRoughness.b;
    float roughness = metallicRoughness.g;

    vec3 lightingResult = calculateLight(norm, metallic, roughness);
    vec4 textureColor = texture(texSampler, uv);
    vec3 result = lightingResult * textureColor.rgb;

    outColor = vec4(result, 1.0);
}