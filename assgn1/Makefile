objects = main.o genmatvec.o matvecres.o
executable = matvec
CFLAGS+=-Wall -std=gnu99

build : $(objects)
	mpicc $(CFLAGS) -o $(executable) $(objects)

test : build
	mpirun -np 4 ./$(executable) 10 10 > test;
	echo "octaveResult = A*b" >> test; \
	echo "errorThreshold = 1.0e-12" >> test; \
    echo "diff = abs(octaveResult - result)" >> test; \
	echo "all(diff < errorThreshold)" >> test; \
	octave test;

.PHONY : clean
clean :
	-rm matvec $(objects)
	
