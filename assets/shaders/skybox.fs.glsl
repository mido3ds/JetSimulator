#version 330 core
// in
in VS_OUT {
    vec3 texCoord;
} from_vs;
// out
out vec4 outFragColor;
// uniforms
uniform samplerCube uCubeMap;
uniform bool uUseFog;

void main() {
    vec4 color = texture(uCubeMap, from_vs.texCoord);

    if(uUseFog) {
        float fogDenisty = 0.005f;
        vec4 fog_color = vec4(0.5f, 0.5f, 0.5f,1.0f);
        float distY = abs(gl_FragCoord.y);
        float distX = abs(gl_FragCoord.x);
        float fogFactorY = exp(-1*distY*fogDenisty);
        float fogFactorX = exp(-1*distX*fogDenisty);
        color = mix(fog_color,color,fogFactorY*fogFactorX);
    }

    outFragColor = color;
}