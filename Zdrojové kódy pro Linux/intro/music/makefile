C=g++
WINEC=wing++

CPAR=#-mmmx -msse3 -O2 -s -Os
CWARN=#-Wall									\
			-W										\
			-pedantic							\
			-Wshadow							\
			-Wpointer-arith


OUTDIR=../output/

CFLAGS=$(CPAR) $(CWARN)

all: ${MYLIB}
	echo "#ifndef _SONGS_H_" > songs.h
	echo "#define _SONGS_H_" >> songs.h
	echo "" >> songs.h
	ls *.v2m | sed "s/\(.*\)\.v2m/extern unsigned char \1_v2m[];/" >> songs.h
	echo "" >> songs.h
	echo "#endif//_SONGS_H_" >> songs.h
	echo "#include\"songs.h\"" > songs.c
	echo "" >> songs.c
	for i in `ls *.v2m`; do xxd -i $$i >> songs.c; done
	${C} -c ${CPAR} *.c
	cp *.o ${OUTDIR}

wingcc: *.v2m *.c *.h
	echo "#ifndef _SONGS_H_" > songs.h
	echo "#define _SONGS_H_" >> songs.h
	echo "" >> songs.h
	ls *.v2m | sed "s/\(.*\)\.v2m/extern unsigned char \1_v2m[];/" >> songs.h
	echo "" >> songs.h
	echo "#endif//_SONGS_H_" >> songs.h
	echo "#include\"songs.h\"" > songs.c
	echo "" >> songs.c
	for i in `ls *.v2m`; do xxd -i $$i >> songs.c; done
	${WINEC} ${CFLAGS} -c *.c
	cp *.o ${OUTDIR}

${SOURCE}: ${HEADER}

${MYLIB}: *.cpp *.c *.h
	${C} ${CFLAGS} -c *.cpp *.c



clean:
	rm -f *.o
	rm -f tags

