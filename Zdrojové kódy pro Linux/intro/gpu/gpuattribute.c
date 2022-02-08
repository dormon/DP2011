#include"gpuattribute.h"

void gpu_InitAttribute(
		SGPUAttribute*Attribute,
		GLuint Index,
		GLint Size,
		GLuint Type,
		GLboolean Normalized,
		GLsizei Stride,
		GLvoid*Pointer){
	Attribute->Index=Index;
	Attribute->Size=Size;
	Attribute->Type=Type;
	Attribute->Normalized=Normalized;
	Attribute->Stride=Stride;
	Attribute->Pointer=Pointer;
}

void gpu_SetAttribute(SGPUAttribute*Attribute){
	glEnableVertexAttribArray(Attribute->Index);//povolime atribut
	glVertexAttribPointer(
			Attribute->Index,//identifikator promenne v shaderu
			Attribute->Size,//pocet slozek
			Attribute->Type,//typ dat
			Attribute->Normalized,//normaliujeme?
			Attribute->Stride,//prokladani
			Attribute->Pointer);//ukazatel na prvni prvek
}

