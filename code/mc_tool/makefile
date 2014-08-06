CFLAGS =  -Wall -Ofast -O3 -g -c -fopenmp
LDFLAGS = -lm -fopenmp
CC		= g++

OBJSGD = $(SRCSGD:.cc=.o)
OBJPEG = $(SRCPEG:.cc=.o)

HDR = simple_sparse_vec_hash.h WeightVector.h pegasos_optimize.h sgdsvm.h
SRCPEG = simple_sparse_vec_hash.cc  WeightVector.cc pegasos_optimize.cc cmd_line.cc mainpeg.o
SRCSGD = simple_sparse_vec_hash.cc  WeightVector.cc pegasos_optimize.cc cmd_line.cc sgdsvm.cc mainsgd.o

%.o: %.cc $(HDR)
	$(CC) $(CFLAGS) $<
sgdsvm:  $(OBJSGD)
	$(CC) $(OBJSGD) $(LDFLAGS) -o sgdsvm
pegasos: $(OBJPEG)
	$(CC) $(OBJPEG) $(LDFLAGS) -o pegasos
clean:
	rm -f *.o sgdsvm pegasos *~
