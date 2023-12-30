#version 450
#define MAX_LIGHTS 5

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 uv;
layout(location = 3) in vec4 tangent;

layout(location = 0) out vec4 outColor;

struct Light {
    int type;// point = 0, spot = 1
    vec4 color;// rgba: 'a' could be used for another property like light radius
    vec3 position;
    vec3 direction;
    float intensity;
};

layout(set = 1, binding = 0) uniform LightArray {
    Light lights[MAX_LIGHTS];
    vec3 cameraPos;
    int numLightsInUse;
} ubo;

layout(set = 1, binding = 1) uniform sampler2D texSampler;
layout(set = 1, binding = 2) uniform sampler2D normalMapSampler;
layout(set = 1, binding = 3) uniform sampler2D metallicRoughnessMapSampler;

vec3 calculateLight(vec3 norm, vec3 fragPos, vec3 viewDir, float metallic, float roughness) {
    vec3 result = vec3(0.0);
    for (int i = 0; i < ubo.numLightsInUse; i++) {
        Light light = ubo.lights[i];
        vec3 lightDir = normalize(light.position - fragPos);
        vec3 halfDir = normalize(lightDir + viewDir);

        // Ambient component
        vec3 ambient = 0.1 * light.color.rgb * light.intensity;

        // Diffuse reflection (Lambert)
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = (1.0 - metallic) * diff * light.color.rgb * light.intensity;

        // Specular reflection (Blinn-Phong model modified by roughness)
        float spec = pow(max(dot(norm, halfDir), 0.0), mix(8.0, 256.0, roughness));
        vec3 specular = spec * light.color.rgb * light.intensity;

        result += ambient + diffuse + specular;
    }
    return result;
}

void main() {
    vec3 bitangent = cross(normal, tangent.xyz) * tangent.w;
    mat3 TBN = mat3(normalize(tangent.xyz), normalize(bitangent), normalize(normal));
    vec3 sampledNormal = texture(normalMapSampler, uv).rgb * 2.0 - 1.0;
    vec3 norm = normalize(TBN * sampledNormal);

    vec3 viewDir = normalize(ubo.cameraPos - fragPos);
    vec3 metallicRoughness = texture(metallicRoughnessMapSampler, uv).rgb;
    float metallic = metallicRoughness.b;
    float roughness = metallicRoughness.g;

    vec3 lightingResult = calculateLight(norm, fragPos, viewDir, metallic, roughness);
    vec4 textureColor = texture(texSampler, uv);
    vec3 result = lightingResult * textureColor.rgb;

    outColor = vec4(result, 1.0f);
}
