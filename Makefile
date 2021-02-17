
default: ompTest

SRC = main.c funcs.c prim.c
HDR = funcs.h prim.h
INC = 
LIB = -lm
FLAG = -O3 -fopenmp -Wall

ompTest: $(SRC) $(HDR)
	gcc $(FLAG) -o $@ $(SRC) $(INC) $(LIB)

clean:
	rm -f ompTest
