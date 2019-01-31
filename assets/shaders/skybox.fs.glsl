#version 330 core
// in
in VS_OUT {
    vec3 texCoord;
} from_vs;
// out
out vec4 outFragColor;
// constants
const float OUTER_RADIUS = 0.65, INNER_RADIUS = 0.4, INTENSITY = 1;
// uniforms
uniform vec2 uResolution;
uniform bool uUseVignette;
uniform samplerCube uCubeMap;
uniform bool uUseFog;
uniform bool uUseGrayScale;
uniform bool uUseSepia;

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

    if (uUseGrayScale) {
        // Take the average of the colors, there are better techniques but this is sufficient
        float gray_color = (color.r + color.g + color.b) / 3.0; 
        color = vec4(gray_color, gray_color, gray_color, 1.0);
    }

    if(uUseSepia) {
        float sepia_red = color.r *0.393f + color.g *0.769f + color.b *0.189f ;
        float sepia_green = color.r *0.349f + color.g *0.686f + color.b *0.168f ;
        float sepia_blue = color.r *0.272f + color.g *0.534f + color.b *0.131f ;	
        color = vec4(sepia_red , sepia_green , sepia_blue, 1.0);
    }

    if(uUseVignette) {
        vec2 relativePos = gl_FragCoord.xy/uResolution - 0.5;
        float len = length(relativePos);
        // smooth transation from OUTER_RADIUS to innerRad
        float vignette = smoothstep(OUTER_RADIUS, INNER_RADIUS, len); 
        color = mix(color, color * vignette, INTENSITY);
    }

    outFragColor = color;
}