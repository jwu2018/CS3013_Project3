CC=gcc

all: cats_dogs

cats_dogs: cats_dogs.o
	$(CC) -o $@ $^

cats_dogs.o: cats_dogs.c
	$(CC) -c $^ -lpthread -lrt

clean:
	rm cats_dogs.o cats_dogs
