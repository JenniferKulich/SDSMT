#define compiler
CC = mpicc

#compile options
CFLAGS = #-g -Wall

#object files
OBJS = life.o

#Target files
TARGET = life

all: $(TARGET)

$(TARGET): $(OBJS
	$(CC) -o $(TARGET) $(OBJS) -lm

prime.o: life.c
	$(CC) $(CFLAGS) life.c

clean:
	$(RM) *.o *~$(TARGET)
