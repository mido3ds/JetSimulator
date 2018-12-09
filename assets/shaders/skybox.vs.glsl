#version 330 core

layout (location = 0) in vec3 inPos;

out vec3 shTexCoord;

uniform mat4 uProjView;

void main() {
    shTexCoord = inPos;
    gl_Position = (uProjView * vec4(inPos, 1.0)).xyww;
}