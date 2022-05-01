#version 410

in float textureSlot;
in vec2 uv;

uniform sampler2D textureMap;

out vec4 fragColor;

float map(float min, float max, float t) {
    return min + ((max - min) * t);
}

void main (void) {
    float slotSize = 1.0f / 8.0f;
    float row = floor(textureSlot / 8.0f);
    float col = mod(textureSlot, 8.0f);
    float umin = col * slotSize;
    float umax = umin + slotSize;
    float vmin = row * slotSize;
    float vmax = vmin + slotSize;
    vec2 slotPos = vec2(
        map(umin, umax, uv.x),
        map(vmin, vmax, uv.y)
    );
    fragColor = texture(textureMap, slotPos);
}