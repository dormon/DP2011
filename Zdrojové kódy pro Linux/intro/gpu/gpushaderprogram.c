#include"gpushaderprogram.h"

GLuint gpu_CreateShader(char*Text,GLuint Type){
	GLuint Result=glCreateShader(Type);//vytvorime shader
	GLchar*ptr[1]={Text};//kvuli divnemu ukazateli
	glShaderSource(Result,1,(const GLchar**)ptr,NULL);//zdroj
	glCompileShader(Result);//zkompilujeme shader
	int len;
	char BUF[10000];
	glGetShaderiv(Result,GL_INFO_LOG_LENGTH,&len);
	glGetShaderInfoLog(Result,len,NULL,BUF);
	fprintf(stderr,"%s\n",BUF);
	return Result;//navrat shaderu
}

/**
 * @brief Vytvori shader program
 *
 * @param Shader pole shaderu
 * @param NumShader pocet shaderu
 *
 * @return vraci shader program
 */
GLuint gpu_CreateShaderProgram(GLuint*Shader,unsigned NumShader){
	GLuint Result=glCreateProgram();
	for(unsigned i=0;i<NumShader;++i)//postupne pridame shadery
		glAttachShader(Result,Shader[i]);//pridavame
	glLinkProgram(Result);//slinkujeme shadery
	return Result;//navrat programu
}

void gpu_InitShaderProgram(
		SGPUShaderProgram*S,
		char*Vertex,
		char*Geometry,
		char*Fragment,
		char**Attributes,
		unsigned NumAttributes,
		char**Uniforms,
		unsigned NumUniforms,
		char**TextureUnits,
		unsigned NumTextureUnits){
	S->NumShader=0;
	if(Vertex)S->Shader[S->NumShader++]=
		gpu_CreateShader(Vertex,GL_VERTEX_SHADER);
	if(Geometry)S->Shader[S->NumShader++]=
		gpu_CreateShader(Geometry,GL_GEOMETRY_SHADER);
	if(Fragment)S->Shader[S->NumShader++]=
		gpu_CreateShader(Fragment,GL_FRAGMENT_SHADER);

	S->ShaderProgram=gpu_CreateShaderProgram(S->Shader,S->NumShader);
	S->NumAttribute=NumAttributes;
	S->NumUniform=NumUniforms;
	S->NumTextureUnit=NumTextureUnits;
	S->Attributes=(GLuint*)mymem_Alloc(sizeof(GLuint)*NumAttributes);
	S->Uniforms=(GLuint*)mymem_Alloc(sizeof(GLuint)*NumUniforms);
	S->TextureUnits=(SGPUTextureUnit*)mymem_Alloc(sizeof(SGPUTextureUnit)*NumTextureUnits);
	for(unsigned i=0;i<NumAttributes;++i)
		S->Attributes[i]=glGetAttribLocation(S->ShaderProgram,Attributes[i]);
	for(unsigned i=0;i<NumUniforms;++i)
		S->Uniforms[i]=glGetUniformLocation(S->ShaderProgram,Uniforms[i]);
	for(unsigned i=0;i<NumTextureUnits;++i)
		gpu_InitTextureUnit(S->TextureUnits+i,
				glGetUniformLocation(S->ShaderProgram,TextureUnits[i]),i);
}

void gpu_FreeShaderProgram(SGPUShaderProgram*S){
	glDeleteProgram(S->ShaderProgram);//uvolnime shader program
	for(unsigned i=0;i<S->NumShader;++i)//projdeme shadery
		glDeleteShader(S->Shader[i]);//uvolnime shadery
	mymem_Free((void**)&S->Uniforms);//uvolnime uniformy
	mymem_Free((void**)&S->Attributes);//uvolnime atributy
	mymem_Free((void**)&S->TextureUnits);//uvolnime texturovaci jednotky
}
