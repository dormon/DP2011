#version 330
#extension GL_EXT_geometry_shader4:enable
layout(lines)in;
layout(line_strip,max_vertices=2)out;
in vec3 gPos[];
in vec3 gNormal[];
in vec2 gCoord[];
in vec3 gCameraPosition[];
out vec3 fPos;
out vec3 fNormal;
out vec2 fCoord;
out vec3 fCameraPosition;
void main(){
	for(int i=0;i<2;++i){
		gl_Position=gl_PositionIn[i];	
		EmitVertex();
	}
	EndPrimitive();
}
 
