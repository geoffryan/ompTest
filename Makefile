
default: ompTest

SRC = main.c
INC = 
LIB = -lm
FLAG = -O3 -fopenmp

ompTest: 
	gcc $(FLAG) -o $@ $(SRC) $(INC) $(LIB)

clean:
	rm -f ompTest
