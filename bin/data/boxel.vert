#version 410
/*
0: position
1: color
2: normal
3: texcoord
*/
layout(location=0) in vec3 aVertex;
layout(location=3) in vec2 aUV;

layout(location=10) in vec3 aPosition;
layout(location=11) in float aLocalOffset;
layout(location=12) in float aLocalRotation;
layout(location=13) in float aPalleteColor;
uniform mat4 modelViewProjectionMatrix;
uniform vec3 localOffsetTable[6];
uniform mat4 localRotationTable[6];
uniform vec4 palletColorTable[10];

out vec4 color;
//out vec2 uv;

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
    mat4 localRotation = localRotationTable[int(aLocalRotation)];
    vec3 position = aPosition + localOffset;
    mat4 localTransform = translate(position) * localRotation * translate(-position);
    mat4 MVP = (modelViewProjectionMatrix * localTransform);
    color = palletColorTable[int(aPalleteColor)];
    gl_Position = MVP * vec4(aVertex + position, 1);
}
