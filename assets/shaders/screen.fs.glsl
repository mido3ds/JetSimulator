#version 330 core

in vec2 shTexCoords;

out vec4 outFragColor;

uniform sampler2D uScreenTexture;

void main() { 
    outFragColor = texture(uScreenTexture, shTexCoords);
}