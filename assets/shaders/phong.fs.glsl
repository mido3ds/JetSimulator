#version 330 core
// defs
#define MAX_POINT_LIGHTS 5
#define MAX_SPOT_LIGHTS  5
//constants
const float outerRadius = 0.65, innerRadius = 0.4, intensity = 1;

// struct
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
struct DirLight {
    vec3 /*normalized*/ dir; // light -> fragment
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight {
    vec3 pos;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float quadratic, linear, constant; 
};
struct SpotLight {
    vec3 pos;
    vec3 /*normalized*/ dir; // light -> fragment
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float innerCone, outerCone;   
    float quadratic, linear, constant; 
};
// function
vec3 DirLight_calc(DirLight self, vec3 normal, vec3 viewToFragDir, float shininess, vec3 diffMap, vec3 specMap);
vec3 PointLight_calc(PointLight self, vec3 normal, vec3 viewToFragDir, float shininess, vec3 fragPos, vec3 diffMap, vec3 specMap);
vec3 SpotLight_calc(SpotLight self, vec3 normal, vec3 viewToFragDir, float shininess, vec3 fragPos, vec3 diffMap, vec3 specMap);
vec3 Fog_calc(vec3 fragPos,vec3 color,DirLight main);
vec4 Vignette_calc(vec4 color);
// in
in VS_OUT {    
    vec3 fragPos;
    vec3 /*normalized*/ normal;
    vec2 texCoord;
} fs_in;
// out
out vec4 outFragCol;
// uniform
uniform Material uMaterial;
uniform vec3 uViewPos;
uniform DirLight uDirLight;
uniform PointLight uPointLights[MAX_POINT_LIGHTS];
uniform SpotLight uSpotLights[MAX_SPOT_LIGHTS];
uniform int uNumPointLights;
uniform int uNumSpotLights;
uniform bool uUseFog;
uniform bool uUseVignette;
uniform vec2 uResolution;
void main() {
    vec3 viewToFragDir = normalize(uViewPos - fs_in.fragPos);
    vec3 diffMap = texture(uMaterial.diffuse, fs_in.texCoord).rgb;
    vec3 specMap = texture(uMaterial.specular, fs_in.texCoord).rgb;
    vec3 color = vec3(0);
    
    color += DirLight_calc(uDirLight, fs_in.normal, viewToFragDir, uMaterial.shininess, diffMap, specMap);
    for (int i = 0; i < uNumPointLights; i++) {
        color += PointLight_calc(uPointLights[i], fs_in.normal, viewToFragDir, uMaterial.shininess, fs_in.fragPos, diffMap, specMap);
    }
    for (int i = 0; i < uNumSpotLights; i++) {
        color += SpotLight_calc(uSpotLights[i], fs_in.normal, viewToFragDir, uMaterial.shininess, fs_in.fragPos, diffMap, specMap);
    }
    if(uUseFog)
    {
		color = Fog_calc(fs_in.fragPos,color,uDirLight);
    }
	outFragCol = vec4(color, 1);
	if(uUseVignette)
    {
		outFragCol = Vignette_calc(outFragCol);
    }
}


vec3 Fog_calc(vec3 fragPos,vec3 color,DirLight main){
    float fogDenisty=0.001f;
    vec3 fog_color = vec3(0.5f, 0.5f, 0.5f)*(main.ambient+main.diffuse);
    float distY=abs(uViewPos.y-fragPos.y);
    float distX=abs(uViewPos.x-fragPos.x);
    float fogFactorY = exp(-1*distY*fogDenisty);
	float fogFactorX = exp(-1*distX*fogDenisty);
    fogFactorY=clamp(fogFactorY,0.0,1.0);
    fogFactorX=clamp(fogFactorX,0.0,1.0);
    return mix(fog_color,color,fogFactorX*fogFactorY);
}
vec3 DirLight_calcDiffuse(DirLight self, vec3 normal) {
    return self.diffuse * max(dot(normal, -self.dir),0);
}
vec3 DirLight_calcSpecular(DirLight self, vec3 normal, vec3 viewToFragDir, float shininess) {
    return self.specular * pow(
        max(
            dot(viewToFragDir,
                normalize(viewToFragDir + self.dir)),
            0),
        shininess
    );
}
vec3 DirLight_calc(DirLight self, vec3 normal, vec3 viewToFragDir, float shininess, vec3 diffMap, vec3 specMap) {
    return self.ambient * diffMap + 
        DirLight_calcDiffuse(self, normal) * diffMap +
        DirLight_calcSpecular(self, normal, viewToFragDir, shininess) * specMap;
}
vec3 PointLight_calcDiffuse(PointLight self, vec3 normal, vec3 /*normalized*/lightToFragDir) {
    return self.diffuse * max(dot(normal, -lightToFragDir),0);
}
vec3 PointLight_calcSpecular(PointLight self, vec3 normal, vec3 viewToFragDir, float shininess, vec3 /*normalized*/lightToFragDir) {
    return self.specular * pow(
        max(
            dot(viewToFragDir,
                normalize(viewToFragDir + lightToFragDir)),
            0),
        shininess
    );
}
float PointLight_calcAtten(PointLight self, float lightToFragDist) {
    return 1.0/(self.constant +
            self.linear * lightToFragDist +
            self.quadratic * (lightToFragDist * lightToFragDist));
}
vec3 PointLight_calc(PointLight self, vec3 normal, vec3 viewToFragDir, float shininess, vec3 fragPos, vec3 diffMap, vec3 specMap) {
    vec3 lightToFrag = self.pos - fragPos;
    vec3 lightToFragDir = normalize(lightToFrag);
    float lightToFragDist = length(lightToFrag);

    return PointLight_calcAtten(self, lightToFragDist) * (self.ambient * diffMap + 
        PointLight_calcDiffuse(self, normal, lightToFragDir) * diffMap +
        PointLight_calcSpecular(self, normal, viewToFragDir, shininess, lightToFragDir) * specMap);
}
vec3 SpotLight_calcDiffuse(SpotLight self, vec3 normal, vec3 /*normalized*/lightToFragDir) {
    return self.diffuse * max(dot(normal, -lightToFragDir),0);
}
vec3 SpotLight_calcSpecular(SpotLight self, vec3 normal, vec3 viewToFragDir, float shininess, vec3 /*normalized*/lightToFragDir) {
    return self.specular * pow(
        max(
            dot(viewToFragDir,
                normalize(viewToFragDir + lightToFragDir)),
            0),
        shininess
    );
}
float SpotLight_calcAtten(SpotLight self, float lightToFragDist) {
    return 1.0/(self.constant +
            self.linear * lightToFragDist +
            self.quadratic * (lightToFragDist * lightToFragDist));
}
vec3 SpotLight_calc(SpotLight self, vec3 normal, vec3 viewToFragDir, float shininess, vec3 fragPos, vec3 diffMap, vec3 specMap) {
    vec3 lightToFrag = self.pos - fragPos;
    vec3 lightToFragDir = normalize(lightToFrag);

    float theta = dot(lightToFragDir, self.dir);
    if (theta > self.outerCone) {
        float intesity = clamp((theta - self.outerCone)/(self.innerCone-self.outerCone), 0, 1);
        float lightToFragDist = length(lightToFrag);
        
        return intesity * SpotLight_calcAtten(self, lightToFragDist) * (self.ambient * diffMap + 
            SpotLight_calcDiffuse(self, normal, lightToFragDir) * diffMap +
            SpotLight_calcSpecular(self, normal, viewToFragDir, shininess, lightToFragDir) * specMap);
    } else {
        return self.ambient * diffMap;
    }
}

vec4 Vignette_calc(vec4 color) {
	vec2 relativePos = gl_FragCoord.xy/uResolution - 0.5;
	float len = length(relativePos);
	float vignette = smoothstep(outerRadius, innerRadius, len); // smooth transation from outerRadius to innerRad
	color.rgb = mix(color.rgb, color.rgb * vignette, intensity);
	return color;
}