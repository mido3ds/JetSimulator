#version 330 core
// in
layout (location=0) in vec3 inPos;
layout (location=1) in vec3 inNormal;
layout (location=2) in vec2 inTexCoord;
// out
out vec3 shFragPos;
out vec3 shNormal;
out vec2 shTexCoord;
// uniform
uniform mat4 uProjView;
uniform mat4 uModel;

void main() {
    vec4 modeledPos = uModel * vec4(inPos, 1);
    shFragPos = modeledPos.xyz;
    shNormal = normalize(mat3(transpose(inverse(uModel))) * inNormal);
    shTexCoord = inTexCoord;

    gl_Position = uProjView * modeledPos;
}