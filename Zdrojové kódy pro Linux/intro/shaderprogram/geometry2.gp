#version 330
#extension GL_EXT_geometry_shader4:enable
layout(triangles)in;
layout(triangle_strip,max_vertices=3)out;
uniform float Smooth;
in vec3 gPos[];
in vec3 gNormal[];
in vec2 gCoord[];
in vec3 gCameraPosition[];
out vec3 fPos;
out vec3 fNormal;
out vec2 fCoord;
out vec3 fCameraPosition;
void main(){
	vec3 FlatNormal=gNormal[0]+gNormal[1]+gNormal[2];
	FlatNormal=normalize(cross((gPos[0]-gPos[1]),(gPos[2]-gPos[0])));
	for(int i=0;i<3;++i){
		gl_Position=gl_PositionIn[i];	
		fPos=gPos[i];
		fNormal=mix(FlatNormal,gNormal[i],Smooth);
		fCoord=gCoord[i];
		fCameraPosition=gCameraPosition[i];
		EmitVertex();
	}
	EndPrimitive();
}
 
