CC = g++
CFLAGS = -Wall -O3
SRC = src
OBJ = obj
INC = inc

boruvka: $(SRC)/boruvka.cpp $(INC)/*
	$(CC) $(CFLAGS) -I $(INC) -o boruvka $(SRC)/boruvka.cpp

clean:
	rm -f boruvka