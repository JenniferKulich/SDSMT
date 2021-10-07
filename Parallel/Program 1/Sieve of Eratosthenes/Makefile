# Use the GNU C/C++ compiler:
CC = gcc
CPP = g++

# COMPILER OPTIONS:
CFLAGS = -c #-g -Wall

#OBJECT FILES
OBJS = Prime.o

#TARGET FILES
TARGET = prime

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -fopenmp -o $(TARGET) $(OBJS) -lm

prime.o: Prime.c
	$(CC) $(CFLAGS) Prime.c

clean:
	$(RM) *.o *~ $(TARGET)
