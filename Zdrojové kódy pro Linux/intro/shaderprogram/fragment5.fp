#version 330
layout(location=0)out vec4 fragColor;
uniform samplerCube SkyBox;
in vec3 fNor;
void main(){
	fragColor=texture(SkyBox,-normalize(fNor));
}
 
