objects = main.o genmatvec.o
CFLAGS+=-Wall -std=gnu99

build : $(objects)
	mpicc $(CFLAGS) -o matvec $(objects)

.PHONY : clean
clean :
	-rm matvec $(objects)
	
