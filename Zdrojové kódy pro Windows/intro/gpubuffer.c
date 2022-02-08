#include"gpubuffer.h"

void gpu_InitBuffer(SGPUBuffer*Buffer,unsigned Size,GLenum Target,GLenum Usage){
	Buffer->DataChanged=0;//data se nezmenili
	Buffer->Target=Target;//typ bufferu (vertex/element/...)
	Buffer->Usage=Usage;//pouziti buffer (rychle vykreslovani/obnova)
	Buffer->Size=Size;//velikost v byte
	Buffer->Data=mymem_Alloc(Buffer->Size);//alokujeme CPU data
	glGenBuffers(1,&Buffer->Buffer);//generujeme GPU buffer
	glBindBuffer(Buffer->Target,Buffer->Buffer);//pribindijeme buffer
	glBufferData(Buffer->Target,Buffer->Size,NULL,Buffer->Usage);//alokujeme data
}

void gpu_FreeBuffer(SGPUBuffer*Buffer){
	glDeleteBuffers(1,&Buffer->Buffer);//udstanime gpu buffer
	mymem_Free(&Buffer->Data);//uvolnime CPU data
}

void gpu_BindBuffer(SGPUBuffer*Buffer){
	glBindBuffer(Buffer->Target,Buffer->Buffer);//pribindujeme buffer
	if(Buffer->DataChanged){//pokud se data zmenily
		glBufferSubData(Buffer->Target,0,Buffer->Size,Buffer->Data);//prekopirujeme
		Buffer->DataChanged=0;//odnastavime priznak
	}
}

void gpu_ChangeBuffer(SGPUBuffer*Buffer){
	Buffer->DataChanged=1;//nastavime priznak
}


void*gpu_MapBuffer(SGPUBuffer*Buffer){
	gpu_BindBuffer(Buffer);//pribindujeme buffer
	return glMapBuffer(Buffer->Target,GL_READ_WRITE);//vracime ukazatel
}

void gpu_UnmapBuffer(SGPUBuffer*Buffer){
	gpu_BindBuffer(Buffer);//pribindujeme buffer
	glUnmapBuffer(Buffer->Target);//zrusime ukazatel
}


