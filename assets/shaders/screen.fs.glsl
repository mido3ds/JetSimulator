#version 330 core

in VS_OUT {
    vec2 texCoords;
} from_vs;

out vec4 outFragColor;

uniform sampler2D uScreenTexture;

void main() { 
    outFragColor = texture(uScreenTexture, from_vs.texCoords);
}