#version 330
#extension GL_EXT_geometry_shader4:enable
layout(points)in;
layout(triangle_strip,max_vertices=4)out;
uniform mat4 ProjectionMatrix;
uniform mat4 CameraMatrix;
uniform float p;
out vec2 c;
out float fTTL;
in float gSize[];
in float gTTL[];
in vec3 gNormal[];
void main(){
	vec3 x=mix(normalize(cross(gNormal[0],vec3(0,1,0))),vec3(1,0,0),p);
	vec3 y=mix(normalize(cross(gNormal[0],x)),vec3(0,1,0),p);
	for(int i=0;i<4;++i){
		vec4 O=gSize[0]*vec4(x*(2*(i%2)-1)+y*(2*(i/2)-1),0);
		gl_Position=ProjectionMatrix*(gl_PositionIn[0]+mix(CameraMatrix*O,O,p));
		fTTL=gTTL[0];
		c=vec2(i%2,i/2);
		EmitVertex();
	}
	EndPrimitive();
}
 
