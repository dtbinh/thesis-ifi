CFLAGS = `pkg-config --cflags opencv` -Wall -g -std=c++11
LDFLAGS = `pkg-config --libs opencv`
CC = g++
OBJ = training.o classificator.o
OBJ2 = predict.o classificator.o
OBJ3 = extractfeatures.o classificator.o
OBJ4 = dir2key.o
OBJ5 = key4kmeans.o
OBJ6 = createcsv.o
OBJ7 = table2libsvm.o
%.o: %.cpp
	$(CC) -c -o $@ $< $(CFLAGS)
all:training predict extractor dir2key key4kmeans createcsv table2libsvm
training:  $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)
predict:  $(OBJ2)
	$(CC) -o $@ $^ $(LDFLAGS)
extractor:  $(OBJ3)
	$(CC) -o $@ $^ $(LDFLAGS)
dir2key:  $(OBJ4)
	$(CC) -o $@ $^ $(LDFLAGS)
key4kmeans:  $(OBJ5)
	$(CC) -o $@ $^ $(LDFLAGS)
createcsv:  $(OBJ6)
	$(CC) -o $@ $^ $(LDFLAGS)
table2libsvm:  $(OBJ7)
	$(CC) -o $@ $^ $(LDFLAGS)
clean:
	rm -f *.o extractor dir2key training predict key4kmeans createcsv table2libsvm *~
