#include <stdio.h>
#include "radixSort.h"
#include "bucketSort.h"
#include <time.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    int size;
    int buckNum;
    if(argc != 2 && argc != 3) {
        size = 100;
        buckNum = 10;
    }
    else if(argc == 3) {
        size = atoi(argv[1]);
        buckNum = atoi(argv[2]);
    }
    else {
        size = atoi(argv[1]);
        buckNum = 10;
    }

    int numbers[size];

    srand(time(NULL));

    for(int i = 0; i < size; i++) {
        int r = rand() % 10000;
        numbers[i] = r;
    }

    printf("Original array: ");
    print(numbers,size);
    // int numbers[] = {123, 8, 40, 67, 298, 76, 8, 93, 30, 77, 43, 345, 157, 187, 265, 3, 36, 300, 40, 50, 86};

    bucketSort(numbers,size,buckNum);

    printf("Sorted Array: ");
    print(numbers,size);

    return 0;
}
