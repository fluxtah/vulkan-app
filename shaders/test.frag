precision mediump float;

// Perlin noise function
float perlinNoise(vec2 p) {
    vec2 i = floor(p);
    vec2 f = fract(p);
    vec2 u = f * f * (3.0 - 2.0 * f);

    float n = i.x + i.y * 57.0;
    vec2 g00 = vec2(sin(n), cos(n));
    vec2 g10 = vec2(sin(n + 1.0), cos(n + 1.0));
    vec2 g01 = vec2(sin(n + 57.0), cos(n + 57.0));
    vec2 g11 = vec2(sin(n + 58.0), cos(n + 58.0));

    float n00 = dot(g00, f - vec2(0.0, 0.0));
    float n10 = dot(g10, f - vec2(1.0, 0.0));
    float n01 = dot(g01, f - vec2(0.0, 1.0));
    float n11 = dot(g11, f - vec2(1.0, 1.0));

    float noise = mix(mix(n00, n10, u.x), mix(n01, n11, u.x), u.y);
    return 0.5 + 0.5 * noise;
}

// Function to combine multiple octaves of noise
float layeredPerlinNoise(vec2 p, const int octaves, float persistence) {
    float total = 0.0;
    float frequency = 1.0;
    float amplitude = 1.0;
    float maxValue = 0.0;  // Used for normalizing result
    const int oct = octaves
    for(int i = 0; i < oct; i++) {
        total += perlinNoise(p * frequency) * amplitude;

        maxValue += amplitude;

        amplitude *= persistence;
        frequency *= 2.0;
    }

    return total / maxValue;
}

void main() {
    float scale = 50.0;  // Smaller value for more detail
    vec2 p = gl_FragCoord.xy / scale;

    float noiseValue = layeredPerlinNoise(p, 4, 0.5); // 4 octaves, persistence of 0.5
    gl_FragColor = vec4(noiseValue, noiseValue, noiseValue, 1.0);
}
