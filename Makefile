OUT=		sequential parallel
SHAREDOBJS=	shared.o
SEQOBJS=	sequential.o $(SHAREDOBJS)
PAROBJS=	parallel.o $(SHAREDOBJS)
TESTOBJS=	test.o $(SHAREDOBJS)

all: $(OUT)

test: CC+= -g -DTEST

sequential: $(SEQOBJS)
	$(CC) -o sequential $(SEQOBJS)

parallel: $(PAROBJS)
	$(CC) -o parallel $(PAROBJS)

test: $(TESTOBJS)
	$(CC) -o test $(TESTOBJS) 

sequential.o: sequential.c sequential.h

parallel.o: parallel.c parallel.h

shared.o: shared.c shared.h

test.o: test.c

clean:
	-rm -f *.o $(OUT) test
