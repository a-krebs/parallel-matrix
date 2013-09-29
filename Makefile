OUT=		sequential parallel
SHAREDOBJS=	shared.o
SEQOBJS=	sequential.o $(SHAREDOBJS)
PAROBJS=	parallel.o $(SHAREDOBJS)
TESTOBJS=	test.o $(SHAREDOBJS)

all: $(OUT)

test: CFLAGS+= -g
test: CFLAGS+= -DTEST

sequential: $(SEQOBJS)
	cc $(CFLAGS) -o sequential $(SEQOBJS)

parallel: $(PAROBJS)
	cc $(CFLAGS) -o parallel $(PAROBJS)

test: $(TESTOBJS)
	cc $(CFLAGS) -o test $(TESTOBJS) 

sequential.o: sequential.c sequential.h

parallel.o: parallel.c parallel.h

shared.o: shared.c shared.h

test.o: test.c

clean:
	-rm -f *.o $(OUT) test
