#Use the GNU C/C++ compiler:
CC = gcc
CPP = g++

# COMPILER OPTIONS:
CFLAGS = -c #-g -Wall

#OBJECT FILES
OBJS = Floyds.o

#TARGET FILES
TARGET = floyds

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -fopenmp -o $(TARGET) $(OBJS) -lm

prime.o: Floyds.c
	$(CC) $(CFLAGS) Floyds.c

clean:
	$(RM) *.o *~$(TARGET)

