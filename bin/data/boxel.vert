#version 410
layout(location=0) in vec3 aVertex;
layout(location=3) in vec2 aUV;

layout(location=10) in vec3 aPosition;
layout(location=11) in float aLocalOffset;
layout(location=12) in float aLocalScale;
layout(location=13) in float aLocalRotation;
layout(location=14) in float aTextureSlot;
uniform mat4 modelViewProjectionMatrix;
uniform vec3 localOffsetTable[36];
uniform vec3 localScaleTable[4];
uniform mat4 localRotationTable[6];

out float textureSlot;
out vec2 uv;

mat4 translate(vec3 v) {
    return mat4(
        vec4(1, 0, 0, 0),
        vec4(0, 1, 0, 0),
        vec4(0, 0, 1, 0),
        vec4(v, 1)
    );
}

void main(void) {
    vec3 localOffset = localOffsetTable[int(aLocalOffset)];
    vec3 localScale = localScaleTable[int(aLocalScale)];
    mat4 localRotation = localRotationTable[int(aLocalRotation)];
    vec3 scaled = localScale * aVertex;
    vec3 position = aPosition + localOffset;
    mat4 localTransform = translate(position) * localRotation * translate(-position);
    mat4 MVP = (modelViewProjectionMatrix * localTransform);
    textureSlot = aTextureSlot;
    uv = aUV;
    gl_Position = MVP * vec4(scaled + position, 1);
}
