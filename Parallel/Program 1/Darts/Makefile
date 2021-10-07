# Use the GNU C/C++ compiler:
CC = gcc
CPP = g++

# COMPILER OPTIONS:
CFLAGS = -c #-g -Wall

#OBJECT FILES
OBJS = Darts.o

#TARGET FILES
TARGET = darts

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -fopenmp -o $(TARGET) $(OBJS) -lm

prime.o: Darts.c
	$(CC) $(CFLAGS) Darts.c

clean:
	$(RM) *.o *~ $(TARGET)
