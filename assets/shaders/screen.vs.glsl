#version 330 core

out VS_OUT {vec2 texCoords;} to_fs;

const vec2 QUAD_POS[6] = vec2[](
    vec2(-1,  1), 
    vec2(-1, -1), 
    vec2(1, -1),  
    vec2(-1,  1), 
    vec2(1, -1),  
    vec2(1,  1)
);

const vec2 QUAD_TEXCOORD[6] = vec2[](
    vec2(0, 1),
    vec2(0, 0),
    vec2(1, 0),
    vec2(0, 1),
    vec2(1, 0),
    vec2(1, 1)
);

void main() {
    gl_Position = vec4(QUAD_POS[gl_VertexID].x, QUAD_POS[gl_VertexID].y, 0.0, 1.0); 
    to_fs.texCoords = QUAD_TEXCOORD[gl_VertexID];
}  