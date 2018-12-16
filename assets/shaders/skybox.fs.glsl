#version 330 core

in vec3 shTexCoord;

out vec4 outFragColor;

uniform samplerCube uCubeMap;
uniform bool uUseFog;

void main() {
    vec4 color=texture(uCubeMap, shTexCoord);
   if(uUseFog) {
    float fogDenisty=0.01f;
    vec4 fog_color = vec4(0.5f, 0.5f, 0.5f,1);
    float distY=abs(gl_FragCoord.y);
    float fogFactor = exp(-1*distY*fogDenisty);
    color = mix(fog_color,color,fogFactor);
   }
    outFragColor = color;
}