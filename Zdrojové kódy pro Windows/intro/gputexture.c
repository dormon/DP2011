#include"gputexture.h"

void gpu_InitTexture(
		SGPUTexture*Texture,
		int MipMap,
		int CubeMap,
		...){
	Texture->CubeMap=CubeMap;
	Texture->DataChanged=1;//data se jeste nezmenila
	glGenTextures(1,&Texture->Texture);//generujeme texturu
	GLenum Smooth=MipMap?GL_LINEAR_MIPMAP_LINEAR:GL_LINEAR;//typ vyhlazovani
	va_list args;
	va_start(args,CubeMap);
	for(unsigned i=0;i<(unsigned)1+5*CubeMap;++i){//projdeme SColorBuffer
		Texture->Data[i]=va_arg(args,SColorBuffer);//SColorBuffer
		GLuint ColorType=ToTextureFormat[Texture->Data[i].Channels-1];//format
		Texture->Target=ToTextureTarget[Texture->Data[i].Size.Dimension-1];//cil
		if(CubeMap)Texture->Target=GL_TEXTURE_CUBE_MAP;//cil je CUBE_MAP
		glBindTexture(Texture->Target,Texture->Texture);//pribindujeme texturu

		if(MipMap)glTexParameteri(Texture->Target,GL_GENERATE_MIPMAP,GL_TRUE);//gen. mipmap

		glTexParameteri(Texture->Target,GL_TEXTURE_MAG_FILTER,GL_LINEAR);//Smooth);//vyhlazeni
		glTexParameteri(Texture->Target,GL_TEXTURE_MIN_FILTER,Smooth);//vyhlazeni

		if(CubeMap)//pokud CUBE_MAP
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,ColorType,
					Texture->Data[i].Size.Size[0],
					Texture->Data[i].Size.Size[1],
					0,ColorType,GL_UNSIGNED_BYTE,NULL);
		else if(Texture->Target==GL_TEXTURE_1D)//1D
			glTexImage1D(Texture->Target,0,ColorType,
					Texture->Data[i].Size.Size[0],
					0,ColorType,GL_UNSIGNED_BYTE,NULL);
		else if(Texture->Target==GL_TEXTURE_2D)//2D
			glTexImage2D(Texture->Target,0,ColorType,
					Texture->Data[i].Size.Size[0],
					Texture->Data[i].Size.Size[1],
					0,ColorType,GL_UNSIGNED_BYTE,NULL);
		else if(Texture->Target==GL_TEXTURE_3D)//3D
			glTexImage3D(Texture->Target,0,ColorType,
					Texture->Data[i].Size.Size[0],
					Texture->Data[i].Size.Size[1],
					Texture->Data[i].Size.Size[2],
					0,ColorType,GL_UNSIGNED_BYTE,NULL);
	}
	va_end(args);
	DrawBar();
}

void gpu_SetTexture(SGPUTexture*Texture){
	glBindTexture(Texture->Target,Texture->Texture);
	if(Texture->DataChanged){//zmenila se textura
		for(unsigned i=0;i<1+5*(Texture->Target==GL_TEXTURE_CUBE_MAP);++i){
			colorbuffer_Update(Texture->Data+i);
			GLuint ColorType=ToTextureFormat[Texture->Data[i].Channels-1];
			if(Texture->Target==GL_TEXTURE_1D)
				glTexSubImage1D(Texture->Target,0,0,
						Texture->Data[i].Size.Size[0],
						ColorType,GL_UNSIGNED_BYTE,
						Texture->Data[i].InterleavedData);
			if(Texture->Target==GL_TEXTURE_2D)
				glTexSubImage2D(Texture->Target,0,0,0,
						Texture->Data[i].Size.Size[0],
						Texture->Data[i].Size.Size[1],
						ColorType,GL_UNSIGNED_BYTE,
						Texture->Data[i].InterleavedData);
			if(Texture->Target==GL_TEXTURE_3D)
				glTexSubImage3D(Texture->Target,0,0,0,0,
						Texture->Data[i].Size.Size[0],
						Texture->Data[i].Size.Size[1],
						Texture->Data[i].Size.Size[2],
						ColorType,GL_UNSIGNED_BYTE,
						Texture->Data[i].InterleavedData);
			if(Texture->Target==GL_TEXTURE_CUBE_MAP)
				glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,0,0,
//						512,512,
						Texture->Data[i].Size.Size[0],
						Texture->Data[i].Size.Size[1],
						ColorType,GL_UNSIGNED_BYTE,
						Texture->Data[i].InterleavedData);
		}
		Texture->DataChanged=0;//data uz jsou obnovena
	}
}

void gpu_FreeTexture(SGPUTexture*Texture){
	glDeleteTextures(1,&Texture->Texture);
	for(unsigned i=0;i<(unsigned)1+Texture->CubeMap*5;++i){
		colorbuffer_Free(Texture->Data+i);
	}
}

void createname(char*OutName,char*Name,unsigned Num){
	int Len=StrLen(Name);
	mymem_Cpy(OutName,Name,Len);
	OutName[Len++]="012345"[Num];
	OutName[Len++]='.';
	OutName[Len++]='t';
	OutName[Len++]='g';
	OutName[Len++]='a';
	OutName[Len++]='\0';
}

void gpu_SaveTexture(const char*Name,SGPUTexture*T){
	if(T->Data[0].Size.Dimension!=2)return;
	unsigned TNum=nsize_Size(&T->Data[0].Size);
	unsigned char*Data=(unsigned char*)
		mymem_Alloc(TNum*4);
	for(unsigned i=0;i<(unsigned)T->CubeMap*5+1;++i){
		char OutName[256];
		createname(OutName,(char*)Name,i);
		FILE*f=fopen(OutName,"wb");
		unsigned char identsize=0;fwrite(&identsize,1,1,f);
		unsigned char colormaptype=0;fwrite(&colormaptype,1,1,f);
		unsigned char imagetype=2;fwrite(&imagetype,1,1,f);
		unsigned short colormapstart=0;fwrite(&colormapstart,2,1,f);
		unsigned short colormaplength=0;fwrite(&colormaplength,2,1,f);
		unsigned char colormapbits=32;fwrite(&colormapbits,1,1,f);
		short xstart=0;fwrite(&xstart,2,1,f);
		short ystart=0;fwrite(&ystart,2,1,f);
		short width=T->Data[i].Size.Size[0];fwrite(&width,2,1,f);
		short height=T->Data[i].Size.Size[1];fwrite(&height,2,1,f);
		unsigned char bits=32;fwrite(&bits,1,1,f);
		unsigned char descriptor=0;fwrite(&descriptor,1,1,f);
		for(unsigned c=0;c<4;++c){
			for(unsigned y=0;y<T->Data[i].Size.Size[1];++y)
				for(unsigned x=0;x<T->Data[i].Size.Size[0];++x){
					if(c>=T->Data[i].Channels){
						Data[(y*width+x)*4+c]=Data[(y*width+x)*4+0];
						Data[(y*width+x)*4+3]=255;
					}else{
						Data[(y*width+x)*4+c]=T->Data[i].Data[c][y*width+x]*255;
					}
				}
		}
		for(unsigned y=0;y<T->Data[i].Size.Size[1];++y)
			for(unsigned x=0;x<T->Data[i].Size.Size[0];++x){
				unsigned char R=Data[(y*width+x)*4];
				Data[(y*width+x)*4]=Data[(y*width+x)*4+2];
				Data[(y*width+x)*4+2]=R;
			}

		fwrite(Data,TNum*4,1,f);

		fclose(f);
	}
}



