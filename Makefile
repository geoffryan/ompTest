include Makefile.in 

default: ompTest

SRC = main.c funcs.c prim.c dom.c
HDR = funcs.h prim.h dom.h
INC = 
LIB = -lm
FLAG = -O3 -fopenmp -Wall $(MACHFLAG)

ompTest: $(SRC) $(HDR)
	$(CC) $(FLAG) -o $@ $(SRC) $(INC) $(LIB)

clean:
	rm -f ompTest
