objects = main.o genmatvec.o matvecres.o
executable = matvec
CFLAGS+=-Wall -std=gnu99

build : $(objects)
	mpicc $(CFLAGS) -o $(executable) $(objects)

test : build
	mpirun -np 4 ./$(executable) > test; \
	echo "octaveResult = A*b" >> test; \
	echo "errorThreshold = 0.00001" >> test; \
	echo "all(octaveResult - result < errorThreshold)" >> test; \
	octave test;

.PHONY : clean
clean :
	-rm matvec $(objects)
	
