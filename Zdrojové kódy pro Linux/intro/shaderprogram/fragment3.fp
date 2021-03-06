#version 330
layout(location=0)out vec4 fragColor;
uniform samplerCube Cube;
uniform sampler3D Wave;
uniform float Time;
uniform vec3 LightColor;
in vec3 fPos;
in vec3 fLightPosition0;
in vec3 fCameraPosition;
void main(){
	vec3 Normal=(texture(Wave,vec3(fPos.xz*10,Time/3)).xzy-.5);
	Normal+=(texture(Wave,vec3(fPos.xz*1,Time/30)).xzy-.5);
	Normal*=.5;
	Normal=normalize(mix(Normal,vec3(0,.5,0),.5));
	vec3 FragmentToLight0=normalize(fLightPosition0-fPos);
	vec3 CameraToFragment=normalize(fPos-fCameraPosition);
	vec3 RCameraToFragment=normalize(reflect(CameraToFragment,Normal));
	float Ambient=.1;
	const float Shininess=30;
	float Lc=max(dot(Normal,FragmentToLight0),0);
	float Diffuse=max(Ambient,Lc);
	float Specular=pow(max(0,dot(RCameraToFragment,FragmentToLight0)),Shininess)*Lc;
	vec3 t=texture(Cube,-RCameraToFragment).xyz;
	fragColor=vec4(t*Diffuse+LightColor*Specular,.3+Specular);
}
 
