#include"gpuframebuffer.h"


void gpu_BindFrameBuffer(unsigned Num,...){
#define NUM_BUF 10
	SGPUFrameBuffer Buffer[NUM_BUF];
	//GLenum List[NUM_BUF];
	va_list args;
	va_start(args,Num);
	for(unsigned i=0;i<Num;++i)
		Buffer[i]=va_arg(args,SGPUFrameBuffer);
	va_end(args);
#undef NUM_BUF
}

