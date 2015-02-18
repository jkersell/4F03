objects = primegap.o processinfo.o result.o
executable = primegap
CFLAGS += -Wall -std=c99 -lgmp

build : $(objects)
	mpicc $(CFLAGS) -o $(executable) $(objects)

sanity : build
	mpirun -np 4 ./$(executable) 2 100

bigtest : build
	mpirun -np 4 ./$(executable) 2 1000

biggesttest : build
	mpirun -np 4 ./$(executable) 2 1543264

moreprocs : build
	mpirun -np 10 ./$(executable) 56 1000000

.PHONY : clean
clean:
	rm $(objects) $(executable)
