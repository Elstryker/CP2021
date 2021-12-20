OBJS	= main.o radixSort.o bucketSort.o
SOURCE	= main.c radixSort.c bucketSort.c
HEADER	= radixSort.h bucketSort.h
OUT	= a.out
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

main.o: main.c
	$(CC) $(FLAGS) main.c -std=c99

radixSort.o: radixSort.c
	$(CC) $(FLAGS) radixSort.c -std=c99

bucketSort.o: bucketSort.c
	$(CC) $(FLAGS) bucketSort.c -std=c99


clean:
	rm -f $(OBJS) $(OUT)

run: $(OUT)
	./$(OUT)
