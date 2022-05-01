#version 410

in vec4 color;
out vec4 fragColor;

void main (void) {
    //vec3 black = vec3(0, 0, 0);
    //vec4 diffuse = texture(uTexture, uv);
    //vec3 color = mix(diffuse.rgb, black, uBrightness);
    fragColor = color;
}