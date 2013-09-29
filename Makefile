OUT=		sequential parallel test
SHAREDOBJS=	shared.o
SEQOBJS=	sequential.o $(SHAREDOBJS)
PAROBJS=	parallel.o $(SHAREDOBJS)
TESTOBJS=	test.o $(SHAREDOBJS)
CFLAGS+=	-g

all: $(OUT)

sequential: $(SEQOBJS)
	cc -g -o sequential $(SEQOBJS)

parallel: $(PAROBJS)
	cc -g -o parallel $(PAROBJS)

test: $(TESTOBJS)
	cc -g -o test $(TESTOBJS)

sequential.o: sequential.c sequential.h

parallel.o: parallel.c parallel.h

shared.o: shared.c shared.h

test.o: test.c

clean:
	-rm -f *.o $(OUT)
