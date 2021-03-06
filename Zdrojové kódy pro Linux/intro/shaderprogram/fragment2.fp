#version 330
out vec4 FragColor;
uniform sampler2D Texture;
uniform sampler2D BumpTexture;
uniform vec3 LightPosition;
in vec3 fCameraPosition;
in vec2 fCoord;
in vec3 fNormal;
in vec3 fPos;
void main(){
	vec3 Normal=fNormal+(texture(BumpTexture,fCoord).xyz*2-1)/5;
	Normal=normalize(Normal);
	vec3 LL=fCameraPosition;
	vec3 FragmentToLight=normalize(LL-fPos);
	vec3 CameraToFragment=normalize(fPos-fCameraPosition);
	vec3 RCameraToFragment=normalize(reflect(CameraToFragment,Normal));
	float Diffuse=max(dot(Normal,FragmentToLight),.1);
	float Specular=pow(max(0,dot(RCameraToFragment,FragmentToLight)),30);
	vec4 t=texture(Texture,fCoord);
	FragColor=t*Diffuse+Specular/6;
}
 
