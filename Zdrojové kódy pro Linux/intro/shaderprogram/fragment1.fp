#version 330
out vec4 o;
uniform sampler2D t;
uniform float MaxTTL;
in float fTTL;
in vec2 c;
uniform float p;
void main(){
	o=texture(t,c)*vec4(1,1,1,fTTL/MaxTTL);
}
 
