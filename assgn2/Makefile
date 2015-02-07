objects = primegap.o
executable = primegap
CFLAGS += -Wall -std=c99

build : $(objects)
	mpicc $(CFLAGS) -o $(executable) $(objects)

.PHONY : clean
clean:
	rm $(objects) $(executable)
