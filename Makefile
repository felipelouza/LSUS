CC = gcc
CFLAGS += -Wall 
#CFLAGS += -g -O0
M64 = 0

CFLAGS += -D_FILE_OFFSET_BITS=64 -DM64=$(M64) -m64 -O3 -fomit-frame-pointer -Wno-char-subscripts 

LFLAGS = -lm -lrt -ldl

LIBOBJ = \
	lib/lsus.o\
	lib/file.o\
	lib/utils.o\
	external/sacak-lcp.o\
	external/malloc_count/malloc_count.o

#	external/gsacak.o\

INPUT = dataset/example.txt
ALG = 0

####

all: main

main: main.c ${LIBOBJ}
		gcc main.c -o main-sus ${LIBOBJ} $(CFLAGS) $(LFLAGS) 

clean:
	\rm -f main-sus lib/*.o external/*.o external/malloc_count/*.o

run:
	./main-sus $(INPUT) -A $(ALG) -t
