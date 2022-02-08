# 64kB opengl intro
# makefile
# @author Tomas Milet, xmilet01
# @file makefile
# @date 23.12.2009
# @version 0.0

ifeq ($(OS),Windows_NT)
	C=g++
	LL=GL/libgdi32.a GL/libopengl32.a GL/libuser32.a GL/libv2.lib -ldsound -lwinmm
else
	C=g++
	LL=-lGL -lGLU -lSDL # -lm
endif

WINEC=wingcc


WAR=#-W -Wall -pedantic
PAR=-mmmx -msse3 -O2 # -s -Os  # -nostdlib -Wl,-emain
DOC=${C} ${PAR} ${WAR}
BIN=intro
REDUCEBIN=smallintro
PACK=terain_xmilet01.tar.gz
COMPILELIB=${wildcard output/*.o}

all:
	${MAKE} -C shaderprogram
	${MAKE} -C adt
	${MAKE} -C index
	${MAKE} -C gen
	${MAKE} -C mymath
	${MAKE} -C std
	${MAKE} -C app
	${MAKE} -C enviroment
	${MAKE} -C marchingtetra
	${MAKE} -C mymem
	${MAKE} -C texturefactory
	${MAKE} -C elastic
	${MAKE} -C gpu
	${MAKE} -C music
	${DOC} output/*.o -o ${BIN} ${LL} #-nostdlib

wingcc:
	${MAKE} wingcc -C shaderprogram
	${MAKE} wingcc -C adt
	${MAKE} wingcc -C index
	${MAKE} wingcc -C gen
	${MAKE} wingcc -C mymath
	${MAKE} wingcc -C std
	${MAKE} wingcc -C app
	${MAKE} wingcc -C enviroment
	${MAKE} wingcc -C marchingtetra
	${MAKE} wingcc -C mymem
	${MAKE} wingcc -C texturefactory
	${MAKE} wingcc -C elastic
	${MAKE} wingcc -C gpu
	${MAKE} wingcc -C music
	${WINEC} output/*.o -o ${BIN} GL/libgdi32.a GL/libopengl32.a GL/libuser32.a GL/libv2.lib -ldsound -lwinmm -s -Os


kill:
	kill -9 `ps -A | grep "intro" | sed "s/^ *//g" | cut  -f1 -d" "`

clean:
	${MAKE} clean -C shaderprogram
	${MAKE} clean -C adt
	${MAKE} clean -C index
	${MAKE} clean -C gen
	${MAKE} clean -C mymath
	${MAKE} clean -C std
	${MAKE} clean -C app
	${MAKE} clean -C enviroment
	${MAKE} clean -C marchingtetra
	${MAKE} clean -C mymem
	${MAKE} clean -C texturefactory
	${MAKE} clean -C elastic
	${MAKE} clean -C gpu
	${MAKE} clean -C music
	rm -f ${BIN} ${REDUCEBIN} ${PACK}
	rm -f output/*.o
	rm -f tags

#RES= 640 480
RES= 1366 768
#RES= 512 512
#RES=768 768
#RES=1024 768
FS= 0

run:all
	./${BIN} ${RES} ${FS}

reduce:
	rm -f ${REDUCEBIN}
	upx ${BIN} -o ${REDUCEBIN}

mem:
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ./${BIN} 800 600 0 32

pack: clean
	tar -cf ${PACK} *

commit: clean
	svn commit .
	svn update
	svn log
	
doc:
	latex mn.tex
	dvips mn.dvi
	ps2pdf mn.ps
	evince mn.pdf
