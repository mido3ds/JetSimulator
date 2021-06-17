#version 330 core

in vec3 shTexCoord;

out vec4 outFragColor;

// constants
const float outerRadius = 0.65, innerRadius = 0.4, intensity = 1, fogDenisty = 0.005f;

// uniforms
uniform vec2 uResolution;
uniform bool uUseVignette;
uniform samplerCube uCubeMap;
uniform bool uUseFog;
uniform bool uUseGrayScale;
uniform bool uUseSepia;

// functions
vec3 Grayscale_calc(vec3 color);
vec3 Sepia_calc(vec3 color);
vec3 Fog_calc(vec3 color);
vec3 Vignette_calc(vec3 color);

void main() {
  vec4 color = texture(uCubeMap, shTexCoord);

  if (uUseFog) {
    color.rgb = Fog_calc(color.rgb);
  }
  if (uUseGrayScale) {
    color.rgb = Grayscale_calc(color.rgb);
  }
  if (uUseSepia) {
    color.rgb = Sepia_calc(color.rgb);
  }
  if (uUseVignette) {
    color.rgb = Vignette_calc(color.rgb);
  }

  outFragColor = color;
}

/*
  effects
*/

vec3 Grayscale_calc(vec3 color) {
  float gray_color = (color.r + color.g + color.b) / 3.0; // Take the average of the colors, there are better techniques but this is sufficient
  return vec3(gray_color, gray_color, gray_color);
}

vec3 Sepia_calc(vec3 color) {
  float sepia_red = color.r *0.393f + color.g *0.769f + color.b *0.189f ;
  float sepia_green = color.r *0.349f + color.g *0.686f + color.b *0.168f ;
  float sepia_blue = color.r *0.272f + color.g *0.534f + color.b *0.131f ;    
  return vec3(sepia_red , sepia_green , sepia_blue);
}

vec3 Fog_calc(vec3 color) {
  vec3 fog_color = vec3(0.5f, 0.5f, 0.5f);
  float distY=abs(gl_FragCoord.y);
  float distX=abs(gl_FragCoord.x);
  float fogFactorY = exp(-1*distY*fogDenisty);
  float fogFactorX = exp(-1*distX*fogDenisty);
  fogFactorY = clamp(fogFactorY,0.0,1.0);
  fogFactorX = clamp(fogFactorX,0.0,1.0);

  return mix(fog_color, color, fogFactorX * fogFactorY);
}

vec3 Vignette_calc(vec3 color) {
  vec2 relativePos = gl_FragCoord.xy/uResolution - 0.5;
  float len = length(relativePos);
  float vignette = smoothstep(outerRadius, innerRadius, len); // smooth transation from outerRadius to innerRad
  return mix(color, color * vignette, intensity);
}
