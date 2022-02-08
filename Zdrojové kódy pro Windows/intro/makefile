SOURCE=${wildcard *.c}
HEADER=${wildcard *.h}
MYLIB=${SOURCE:%.c=%.o}

BIN=Windows7

%.o: %.c
	wing++ -c -o $@ $^ -IGL

all: ${MYLIB}
	wing++ *.o -o ${BIN} GL/libgdi32.a GL/libopengl32.a GL/libuser32.a GL/libv2.lib -ldsound -lwinmm

run: all
	./${BIN} 800 600 0 32 32 32

mem:
	valgrind --tool=memcheck --leak-check=yes --show-reachable=yes ./${BIN} 800 600 0 32 32 32


clean:
	rm *.o
