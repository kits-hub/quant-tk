CC=		cc
CFLAGS=		-g -Wall -O2 -Wno-unused-function
CPPFLAGS=
INCLUDES=   
PROG=		quant-tk
BINDIR=		/usr/local/bin
LIBS=		-lz
OBJS=		idxstats.o abund.o fpkm.o tpm.o colsum.o agg.o joint.o strip.o kstring.o utils.c

.SUFFIXES:.c .o
.PHONY:all clean

.c.o:
		$(CC) -c $(CFLAGS) $(CPPFLAGS) $(INCLUDES) $< -o $@

all:$(PROG)

quant-tk:$(OBJS) quant-tk.o
		$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

install:all
		install quant-tk $(BINDIR)

clean:
		rm -fr *.o quant-tk