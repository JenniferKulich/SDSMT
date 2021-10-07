#define c compiler
CC = mpicc

#compile options
CFLAGS = #-g -Wall

#object files
OBJS = ping_pong.o

#Target files
TARGET = ping_pong

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS) -lm

prime.o: ping_pong.c
	$(CC) $(CFLAGS) ping_pong.c

clean:
	$(RM) *.o *~$(TARGET)

