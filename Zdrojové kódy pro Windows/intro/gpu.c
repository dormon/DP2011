#include"gpu.h"

void gpu_SetMatrix(GLuint*MatrixUniform,STransform Scene,SCam Cam){
	float N[16];
	glUniformMatrix4fv(MatrixUniform[0],1,GL_FALSE,Cam.PM);
	transform_GetModelViewMatrix(N,&(Cam.Transform));
	glUniformMatrix4fv(MatrixUniform[1],1,GL_FALSE,N);
	transform_GetModelViewMatrix(N,&Scene);
	glUniformMatrix4fv(MatrixUniform[2],1,GL_FALSE,N);

}


