OUT=		sequential parallel
SHAREDOBJS=	shared.o
SEQOBJS=	sequential.o $(SHAREDOBJS)
PAROBJS=	parallel.o $(SHAREDOBJS)
TESTOBJS=	test.o $(SHAREDOBJS)

all: CFLAGS+= -O2
all: $(OUT)

test: CFLAGS+= -g -DTEST

sequential: $(SEQOBJS)
	$(CC) -o sequential $(SEQOBJS)

parallel: $(PAROBJS)
	$(CC) -o parallel $(PAROBJS)

test: $(TESTOBJS)
	$(CC) -o test $(TESTOBJS) 

sequential.o: sequential.c

parallel.o: parallel.c

shared.o: .FORCE

.FORCE: shared.c shared.h

test.o: test.c

clean:
	-rm -f *.o $(OUT) test
