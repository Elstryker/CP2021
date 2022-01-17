#include "bucketSort.h"
#include "quickSort.h"


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
    //int * elementBucketAllocation = malloc(len * sizeof(int));
    int * sizeBucket = malloc(numBuckets * sizeof(int));

    int bucketMaxSize = 0.2 * len;
    // Initializing buckets
    for(int i = 0; i < numBuckets; i++) {
        sizeBucket[i] = 0;
        buckets[i] = malloc(bucketMaxSize * sizeof(int));
    }

    // Get min and max to calculate range of buckets
    int * minAndMax = getMinAndMax(arr,len);

    int range = ((minAndMax[1] - minAndMax[0]) / numBuckets) +1;
    
    // Get info for buckets
    for(int i = 0; i < len; i++) {
        // Get bucket number correspondent to current element
        int bucketNum = (arr[i] - minAndMax[0]) / range;
        
        if (sizeBucket[bucketNum] == bucketMaxSize) {
            buckets[bucketNum] = realloc(buckets[bucketNum], 2 * bucketMaxSize * sizeof(int));
        }

        buckets[bucketNum][sizeBucket[bucketNum]] = arr[i];
        sizeBucket[bucketNum]++;

    }
    
    // Sort buckets
    
    #pragma omp parallel for schedule(guided)
    for(int i = 0; i < numBuckets; i++) {

       quickSort(buckets[i],sizeBucket[i]);
    }

    // Merge all bucket elements
    int curr = 0;
    
    for(int buck = 0; buck < numBuckets; buck++) {
        for(int j = 0; j < sizeBucket[buck]; j++) {
            arr[curr++] = buckets[buck][j];
        }
        free(buckets[buck]);
    }

    // Free all memory allocated

    free(buckets);
    //free(elementBucketAllocation);
    free(sizeBucket);
    //free(bucketCurCount);
    free(minAndMax);
}