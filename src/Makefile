.PHONY = all clean
.SHELL = /bin/bash
CC = gcc
CFLAGS = -c -O3 -Wall
LDFLAGS = -lz
BINDIR = ../bin/
EXEC = NTContent
OBJS = $(addsuffix .o, NTContent sequence printNTContent parameters utils)

all: ${BINDIR} $(addprefix ${BINDIR}, ${EXEC})

${BINDIR}:
	mkdir ../bin

%.o: %.c
	$(CC) ${CFLAGS} -o $@ $<

$(addprefix ${BINDIR}, NTContent): ${OBJS}
	$(CC) -o $@ $^ ${LDFLAGS}

clean:
	rm -f $(addprefix ${BINDIR}, ${EXEC}) *.o

NTContent.o: utils.h parameters.h sequence.h printNTContent.h
sequence.o: utils.h parameters.h sequence.h
printNTContent.o: utils.h parameters.h printNTContent.h
parameters.o: utils.h parameters.h
utils.o: utils.h
