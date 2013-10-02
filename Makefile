OUT=		sequential parallel
SHAREDOBJS=	shared.o
SEQOBJS=	sequential.o $(SHAREDOBJS)
PAROBJS=	parallel.o thread.o $(SHAREDOBJS)
TESTOBJS=	test.o $(SHAREDOBJS)

CFLAGS+= -Wall

all: CFLAGS+= -O2
all: $(OUT)

verify: CFLAGS+= -DVERIFY
verify: $(OUT)

debug: CFLAGS+= -g -DDEBUG
debug: $(OUT)

vdebug: CFLAGS+= -g -DDEBUG -DVERIFY
vdebug: $(OUT)

test: CFLAGS+= -g -DTEST

sequential: $(SEQOBJS)
	$(CC) -o sequential $(SEQOBJS)

parallel: $(PAROBJS)
	$(CC) -o parallel $(PAROBJS) -lpthread

test: $(TESTOBJS)
	$(CC) -o test $(TESTOBJS) 

sequential.o: sequential.c

parallel.o: parallel.c

thread.o: thread.c thread.h

shared.o: shared.c shared.h

test.o: test.c

clean:
	-rm -f *.o $(OUT) test
