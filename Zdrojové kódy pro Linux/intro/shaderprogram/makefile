C=g++
WINEC=wing++


CPAR=-std=c99 -mmmx -msse3 -O2 -s -Os

all: *.vp *.gp *.fp
	echo "#ifndef _GLSLPROGRAM_H_" >  glslprogram.h
	echo "#define _GLSLPROGRAM_H_" >> glslprogram.h 
	echo "" >> glslprogram.h
	ls *.vp | sed "s/\(.*\)\.vp/extern unsigned char \1_vp[];/" >> glslprogram.h
	ls *.vp | sed "s/\(.*\)\.vp/extern unsigned \1_vp_len;/" >> glslprogram.h
	ls *.gp | sed "s/\(.*\)\.gp/extern unsigned char \1_gp[];/" >> glslprogram.h
	ls *.gp | sed "s/\(.*\)\.gp/extern unsigned \1_gp_len;/" >> glslprogram.h
	ls *.fp | sed "s/\(.*\)\.fp/extern unsigned char \1_fp[];/" >> glslprogram.h
	ls *.fp | sed "s/\(.*\)\.fp/extern unsigned \1_fp_len;/" >> glslprogram.h
	echo "" >> glslprogram.h
	echo "#endif//_GLSLPROGRAM_H_" >> glslprogram.h
	echo "#include\"glslprogram.h\"" > glslprogram.c
	echo "" >> glslprogram.c
	for i in `ls *.vp`; do xxd -i $$i >> glslprogram.c; done
	for i in `ls *.gp`; do xxd -i $$i >> glslprogram.c; done
	for i in `ls *.fp`; do xxd -i $$i >> glslprogram.c; done
	${C} -c ${CPAR} *.c
	cp *.o ../output

wingcc: *.vp *.gp *.fp
	echo "#ifndef _GLSLPROGRAM_H_" >  glslprogram.h
	echo "#define _GLSLPROGRAM_H_" >> glslprogram.h 
	echo "" >> glslprogram.h
	ls *.vp | sed "s/\(.*\)\.vp/extern unsigned char \1_vp[];/" >> glslprogram.h
	ls *.vp | sed "s/\(.*\)\.vp/extern unsigned \1_vp_len;/" >> glslprogram.h
	ls *.gp | sed "s/\(.*\)\.gp/extern unsigned char \1_gp[];/" >> glslprogram.h
	ls *.gp | sed "s/\(.*\)\.gp/extern unsigned \1_gp_len;/" >> glslprogram.h
	ls *.fp | sed "s/\(.*\)\.fp/extern unsigned char \1_fp[];/" >> glslprogram.h
	ls *.fp | sed "s/\(.*\)\.fp/extern unsigned \1_fp_len;/" >> glslprogram.h
	echo "" >> glslprogram.h
	echo "#endif//_GLSLPROGRAM_H_" >> glslprogram.h
	echo "#include\"glslprogram.h\"" > glslprogram.c
	echo "" >> glslprogram.c
	for i in `ls *.vp`; do xxd -i $$i >> glslprogram.c; done
	for i in `ls *.gp`; do xxd -i $$i >> glslprogram.c; done
	for i in `ls *.fp`; do xxd -i $$i >> glslprogram.c; done
	${WINEC} -c ${CPAR} *.c
	cp *.o ../output

clean:
	rm -f *.o

