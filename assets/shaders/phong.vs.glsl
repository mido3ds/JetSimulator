#version 330 core
// in
layout (location=0) in vec3 inPos;
layout (location=1) in vec3 inNormal;
layout (location=2) in vec2 inTexCoord;
// out
out VS_OUT {    
    vec3 fragPos;
    vec3 normal;
    vec2 texCoord;
} vs_out;
// uniform
uniform mat4 uProjView;
uniform mat4 uModel;

void main() {
    vec4 modeledPos = uModel * vec4(inPos, 1);
    vs_out.fragPos = modeledPos.xyz;
    vs_out.normal = normalize(mat3(transpose(inverse(uModel))) * inNormal);
    vs_out.texCoord = inTexCoord;

    gl_Position = uProjView * modeledPos;
}