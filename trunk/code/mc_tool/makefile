CFLAGS = `pkg-config --cflags opencv` -Wall -g -std=c++11 -fopenmp
LDFLAGS = `pkg-config --libs opencv` -lm -fopenmp
CC = g++
OBJ = sgdsvm.o main.o cmd_line.o simple_sparse_vec_hash.o pegasos_optimize.o  WeightVector.o
%.o: %.cpp
	$(CC) -c $< $(CFLAGS)
sgdsvm:  $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
clean:
	rm -f *.o sgdsvm *~
