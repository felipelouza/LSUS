CC = gcc
CFLAGS += -Wall 
#CFLAGS += -g -O0

CFLAGS += -D_FILE_OFFSET_BITS=64 -m64 -O3 -fomit-frame-pointer -Wno-char-subscripts 

LFLAGS = -lm -lrt -ldl

MALLOC_COUNT = external/malloc_count/malloc_count.o
MALLOC_COUNT64 = external/malloc_count/malloc_count.64.o

LIBOBJ = \
	lib/lsus.o\
	lib/file.o\
	lib/utils.o\
	external/sacak-lcp.o ${MALLOC_COUNT}

LIBOBJ_64 = \
	lib/lsus.64.o\
	lib/file.64.o\
	lib/utils.64.o\
	external/sacak-lcp.64.o ${MALLOC_COUNT64}

TARGETS = lsus lsus-64

####
INPUT = dataset/example.txt
ALG = 3
####

all:${TARGETS}

%.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $< -DM64=0

%.64.o:%.c
	$(CC) $(CFLAGS) -c -o $@ $< -DM64=1


lsus: main.c ${LIBOBJ}
		gcc main.c -o $@ ${LIBOBJ} $(CFLAGS) $(LFLAGS) -DM64=0

lsus-64: main.c ${LIBOBJ_64}
		gcc main.c -o $@ ${LIBOBJ_64} $(CFLAGS) $(LFLAGS) -DM64=1

clean:
	\rm -f lib/*.o external/*.o external/malloc_count/*.o ${TARGETS}

run:
	./lsus $(INPUT) -A $(ALG) -t
