#use the GNU C/C++ compiler:
CC = gcc
CPP = g++

# COMPILER OPTIONS:
CFLAGS = -c #-g -Wall

#OBJECT FILES
OBJS = Banker.o Customer.o

#TARGET FILES
TARGET = bankers

.PHONY: all
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -pthread -o $(TARGET) $(OBJS)

Banker.o: Banker.c Bank.h
	$(CC) $(CFLAGS) Banker.c Bank.h

Customer.o: Customer.c Bank.h
	$(CC) -pthread $(CFLAGS) Customer.c

clean:
	$(RM) *.o *~ $(TARGET)
