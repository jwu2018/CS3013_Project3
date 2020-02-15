CC=gcc

all: cats_dogs

cats_dogs: cats_dogs.o
	$(CC) -o $@ $^

cats_dogs.o: cats_dogs.c
	$(CC) -c $^

clean:
	rm cats_dogs.o cats_dogs
