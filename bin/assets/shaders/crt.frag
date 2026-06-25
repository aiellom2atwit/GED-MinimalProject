#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D texture0;
uniform vec2 resolution;
uniform float time;

void main(){
    vec2 uv = fragTexCoord;

    // --- Barrel/screen curvature ---
    vec2 curved = uv * 2.0 - 1.0;
    vec2 offset = curved.yx * curved.yx * 0.015;
    curved += curved * offset;
    uv = curved * 0.5 + 0.5;

    // Kill pixels outside curved screen edge
    if(uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0){
        finalColor = vec4(0.0, 0.0, 0.0, 1.0);
        return;
    }

    // --- Chromatic aberration (RGB channel split) ---
    float aberration = 0.0008;
    float r = texture(texture0, uv + vec2( aberration, 0.0)).r;
    float g = texture(texture0, uv                        ).g;
    float b = texture(texture0, uv - vec2( aberration, 0.0)).b;
    vec4 color = vec4(r, g, b, 1.0);

    // --- Color bleed (horizontal smear) ---
    float bleed = 0.002;
    color.r += texture(texture0, uv + vec2(bleed * 1.5, 0.0)).r * 0.15;
    color.g += texture(texture0, uv + vec2(bleed * 0.5, 0.0)).g * 0.10;
    color.b += texture(texture0, uv - vec2(bleed * 1.0, 0.0)).b * 0.12;

    // --- Scanlines (chunky, visible, ~240p style) ---
    // Use a low line count to get thick visible lines like Apple II output
    float scanlineCount = 240.0;
    float scanline = sin(uv.y * scanlineCount * 3.14159);
    scanline = pow(abs(scanline), 0.4); // soften
    color.rgb *= 0.75 + 0.25 * scanline;

    // --- Phosphor glow (bright areas bleed slightly) ---
    float brightness = dot(color.rgb, vec3(0.299, 0.587, 0.114));
    color.rgb += brightness * brightness * 0.15;

    // --- Subtle noise/grain ---
    float noise = fract(sin(dot(uv + time * 0.001, vec2(12.9898, 78.233))) * 43758.5453);
    color.rgb += (noise - 0.5) * 0.03;

    // --- Vignette ---
    vec2 vignUV = uv * (1.0 - uv.yx);
    float vignette = pow(vignUV.x * vignUV.y * 12.0, 0.35);
    color.rgb *= vignette;

    // --- Slight green phosphor tint (Apple II feel) ---
    // Uncomment if you want the classic green tint
    // color.rgb *= vec3(0.9, 1.05, 0.9);

    finalColor = vec4(color.rgb, 1.0);
}