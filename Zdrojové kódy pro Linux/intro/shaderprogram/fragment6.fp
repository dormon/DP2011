#version 330
out vec4 FragColor;
uniform sampler2D Texture;
in vec3 fCameraPosition;
in vec2 fCoord;
in vec3 fNormal;
in vec3 fPos;
void main(){
	FragColor=vec4(1,1,1,.3);
}
 
