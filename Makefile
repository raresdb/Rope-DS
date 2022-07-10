#Butilca Rares

# compiler
CC=gcc

# define target
TARGET=rope

build: $(TARGET)

rope: rope.c
	$(CC) rope.c -o rope

clean:
	rm -f $(TARGET)
