#version 330 core

in vec2 inTexCoords;

out vec4 shFragColor;

uniform sampler2D uScreenTexture;

void main() { 
    shFragColor = texture(uScreenTexture, inTexCoords);
}