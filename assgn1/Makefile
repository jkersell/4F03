objects = main.o genmatvec.o matvecres.o
executable = matvec
octave = octave -q
CFLAGS+=-Wall -std=gnu99

build : $(objects)
	mpicc $(CFLAGS) -o $(executable) $(objects)

test : build
	mpirun -np 4 ./$(executable) 10 10 | $(octave);


.PHONY : clean
clean :
	-rm matvec $(objects) octave.result
	
