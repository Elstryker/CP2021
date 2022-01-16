# source files.
SRC = main.c radixSort.c bucketSort_parallel.c 

OBJ = $(SRC:.cpp=.o)

OUT = test

# include directories
INCLUDES = -I.
 
# C compiler flags 
CCFLAGS = -O2 -Wall -I/share/apps/papi/5.4.1/include -ftree-vectorize -msse4 -std=c99

# compiler
CCC = gcc 

# libraries
LIBS = -L/share/apps/papi/5.4.1/lib -lm -lpapi -fopenmp

.SUFFIXES: .cpp .c 


default: $(OUT)

.cpp.o:
	$(CCC) $(CCFLAGS) $(INCLUDES)  -c $< -o $@

.c.o:
	$(CCC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

$(OUT): $(OBJ)
	$(CCC) -o $(OUT) $(CCFLAGS) $(OBJ) $(LIBS) 

depend:  dep
#
#dep:
#	makedepend -- $(CFLAGS) -- $(INCLUDES) $(SRC)

clean:
	rm -f *.o .a *~ Makefile.bak $(OUT)
