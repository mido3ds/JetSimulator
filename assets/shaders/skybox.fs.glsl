#version 330 core

in vec3 shTexCoord;

out vec4 outFragColor;

uniform samplerCube uCubeMap;

void main() {
    outFragColor = texture(uCubeMap, shTexCoord);
}