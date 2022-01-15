#include "bucketSort.h"
#include "bubbleSort.h"

void print(int arr[], int n) {
    for (int i = 0; i < n; i++)
        printf("%d ",arr[i]);
    printf("\n");
}

int * getMinAndMax(int arr[], int len) {
    int * minAndMax = malloc(2 * sizeof(int));
    minAndMax[0] = minAndMax[1] = arr[0];
    for(int i = 1; i < len; i++) {
        minAndMax[0] = arr[i] < minAndMax[0] ? arr[i] : minAndMax[0];
        minAndMax[1] = arr[i] > minAndMax[1] ? arr[i] : minAndMax[1];
    }
    return minAndMax;
}

void bucketSort(int arr[], int len, int numBuckets) {
    int ** buckets = malloc(numBuckets * sizeof(int *));
    int * elementBucketAllocation = malloc(len * sizeof(int));
    int * sizeBucket = malloc(numBuckets * sizeof(int));

    // Initializing buckets
    for(int i = 0; i < numBuckets; i++) {
        sizeBucket[i] = 0;
    }

    // Get min and max to calculate range of buckets
    int * minAndMax = getMinAndMax(arr,len);

    int range = (int) ceil((1.0*(minAndMax[1] - minAndMax[0])) / numBuckets);
    
    // Get info for buckets
    for(int i = 0; i < len; i++) {
        // Get bucket number correspondent to current element
        int bucketNum = (int) floor((arr[i] - minAndMax[0]) / range);
        float diff = (1.0*(arr[i] - minAndMax[0])) / range - (int) (floor((arr[i] - minAndMax[0]) / range));
        // See if current element is a case that is divided between two buckets
        if(diff == 0 && arr[i] != minAndMax[0]) {
            bucketNum -= 1;
        }

        // Increase bucket size
        (sizeBucket[bucketNum])++;
        // Select bucket location
        elementBucketAllocation[i] = bucketNum;

    }
    
    // Pointer for adding elements in each bucket
    int * bucketCurCount = malloc(numBuckets * sizeof(int));

    // Initialize buckets
    for(int i = 0; i < numBuckets; i++) {
        buckets[i] = malloc(sizeBucket[i] * sizeof(int));
        bucketCurCount[i] = 0;
    }

    // Fill buckets
    for(int i = 0; i < len; i++) {
        int bucketNum = elementBucketAllocation[i];
        buckets[bucketNum][bucketCurCount[bucketNum]] = arr[i];
        (bucketCurCount[bucketNum])++;
    }

    // See bucket composition
    for(int i = 0; i < numBuckets; i++) {
        printf("Bucket %d, Count %d : ", i, sizeBucket[i]);
        print(buckets[i],sizeBucket[i]);
    }

    // Sort buckets
    for(int i = 0; i < numBuckets; i++) {
        bubbleSort(buckets[i],sizeBucket[i]);
    }

    // Merge all bucket elements
    int curr = 0;
    for(int buck = 0; buck < numBuckets; buck++) {
        for(int j = 0; j < sizeBucket[buck]; j++) {
            arr[curr++] = buckets[buck][j];
        }
    }

    // Free all memory allocated
    for(int i = 0; i < numBuckets; i++) {
        free(buckets[i]);
    }
    free(buckets);
    free(elementBucketAllocation);
    free(sizeBucket);
    free(bucketCurCount);
    free(minAndMax);
}