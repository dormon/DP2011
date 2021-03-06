#version 330
layout(location=0)out vec4 fragColor;
uniform sampler2D TerrainBump;
uniform sampler1D HeightTexture;
uniform sampler1D RockTexture;
uniform float Time;
uniform float MaxHeight;
uniform float SeaHeight;
uniform float MountainHeight;
uniform float MinHeight;
uniform vec3 LightColor;
uniform vec3 FogColor;
in vec3 fPos;
in vec3 fNor;
in vec2 fCoord;
in vec3 fLightPosition0;
in vec3 fCameraPosition;
void main(){
	vec3 Bump=normalize(texture(TerrainBump,fCoord).xyz*2-1).xzy*vec3(1,-1,1);
	mat3 S;
	S[2]=normalize(fNor);
	S[1]=normalize(cross(S[2],vec3(1,0,0)));
	S[0]=normalize(cross(S[2],S[1]));
	Bump=S*Bump;

	float h=(fPos.y-MinHeight)/(MaxHeight-MinHeight);
	vec3 Normal=normalize(mix(Bump,fNor,.5));

	vec3 FragmentToLight0=normalize(fLightPosition0-fPos);
	vec3 CameraToFragment=normalize(fPos-fCameraPosition);
	vec3 RCameraToFragment=normalize(reflect(CameraToFragment,Normal));
	float Ambient=0.1;
	const float Shininess=30;
	float Lc=max(dot(Normal,FragmentToLight0),0);
	Lc*=max(dot(fNor,FragmentToLight0),0);

	float Diffuse=Lc;
	float Specular=pow(max(0,dot(RCameraToFragment,FragmentToLight0)),Shininess)*Lc;

	float mp=smoothstep(MountainHeight-.2,MountainHeight,h);
	float sp=1-smoothstep(SeaHeight-.2,SeaHeight,h);

	vec3 HeightT=texture(HeightTexture,h).xyz;
	vec3 RockT=texture(RockTexture,h).xyz;
	vec3 t=mix(HeightT,RockT,pow(1-max(Normal.y,0),1));

	fragColor=vec4(mix(
	t*(LightColor*Diffuse+vec3(Ambient))+Specular*mp/2,FogColor,
	smoothstep(.98*pow(h,.1),1,gl_FragCoord.z)*.4),1);
}
 
