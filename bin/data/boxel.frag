#version 410

in vec2 uv;

uniform sampler2D textureMap;

out vec4 fragColor;

void main (void) {
    fragColor = texture(textureMap, uv);
}