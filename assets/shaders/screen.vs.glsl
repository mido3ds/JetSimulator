#version 330 core

layout (location = 0) in vec2 inPos;
layout (location = 1) in vec2 inTexCoords;

out VS_OUT {
    vec2 texCoords;
} to_fs;

void main() {
    gl_Position = vec4(inPos.x, inPos.y, 0.0, 1.0); 
    to_fs.texCoords = inTexCoords;
}  