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

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float calculateAttenuation(vec3 lightPos, vec3 fragPos) {
    float distance = length(lightPos - fragPos);
    float attenuation = 1.0 / (distance * distance);
    return attenuation;
}

vec3 calculateLight(vec3 norm, vec3 fragPos, vec3 viewDir, float metallic, float roughness, vec3 textureColor) {
    vec3 result = vec3(0.0);
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, textureColor, metallic);

    for (int i = 0; i < ubo.numLightsInUse; i++) {
        Light light = ubo.lights[i];
        vec3 lightDir = normalize(light.position - fragPos);
        vec3 halfDir = normalize(lightDir + viewDir);

        vec3 fresnel = fresnelSchlick(max(dot(viewDir, norm), 0.0), F0);

        // Adjusted Ambient Component
        vec3 ambient = 0.03 * light.color.rgb * light.intensity;

        // Diffuse Reflection
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = (1.0 - metallic) * diff * light.color.rgb * light.intensity;
        diffuse *= (1.0 - fresnel);

        // Specular Reflection
        float spec = pow(max(dot(norm, halfDir), 0.0), mix(16.0, 256.0, roughness)); // Increased spec power
        vec3 specular = spec * light.color.rgb * light.intensity * fresnel;

        // Use the calculateAttenuation function
        float attenuation = calculateAttenuation(light.position, fragPos);
        diffuse *= attenuation;
        specular *= attenuation;

        result += ambient + diffuse + specular;
    }
    return result;
}

vec3 gammaCorrect(vec3 color) {
    return pow(color, vec3(1.0/2.2));
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

    vec4 textureColor = texture(texSampler, uv);
    vec3 lightingResult = calculateLight(norm, fragPos, viewDir, metallic, roughness, textureColor.rgb);

    // Apply Gamma Correction
    vec3 result = gammaCorrect(lightingResult * textureColor.rgb);

    outColor = vec4(result, 1.0f);
}
