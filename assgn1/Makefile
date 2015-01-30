objects = main.o genmatvec.o matvecres.o
tests = sanity large very_large prime different moreProcs everMoreProcs moreProcsThanRows
executable = matvec
octave = octave -q
CFLAGS+=-Wall -std=gnu99

build : $(objects)
	mpicc $(CFLAGS) -o $(executable) $(objects)

sanity : build
	mpirun -np 4 ./$(executable) 10 10 | $(octave);

large : build
	mpirun -np 4 ./$(executable) 100 100 | $(octave);

very_large : build
	mpirun -np 4 ./$(executable) 1000 1000 | $(octave);

prime : build
	mpirun -np 4 ./$(executable) 251 251 | $(octave);
	
different : build
	mpirun -np 4 ./$(executable) 34 12 | $(octave);

moreProcs : build
	mpirun -np 8 ./$(executable) 100 100 | $(octave);

everMoreProcs : build
	mpirun -np 32 ./$(executable) 100 100 | $(octave);

moreProcsThanRows : build
	mpirun -np 32 ./$(executable) 10 10 | $(octave);

runAllTests : build $(tests)


.PHONY : clean
clean :
	-rm $(executable) $(objects) octave.result octave-workspace
	
