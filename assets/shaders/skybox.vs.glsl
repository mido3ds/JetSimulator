#version 330 core

layout (location = 0) in vec3 inPos;

out VS_OUT {
    vec3 texCoord;
} to_fs;

uniform mat4 uProjView;

void main() {
    to_fs.texCoord = inPos;
    gl_Position = (uProjView * vec4(inPos, 1.0)).xyww;
}