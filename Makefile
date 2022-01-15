OBJS	= main.o bubbleSort.o bucketSort.o
SOURCE	= main.c bubbleSort.c bucketSort.c
HEADER	= bubbleSort.h bucketSort.h
OUT	= a.out
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c -std=c99

bubbleSort.o: bubbleSort.c
	$(CC) $(FLAGS) bubbleSort.c -std=c99

bucketSort.o: bucketSort.c
	$(CC) $(FLAGS) bucketSort.c -std=c99


clean:
	rm -f $(OBJS) $(OUT) *.o

run: $(OUT)
	./$(OUT)
