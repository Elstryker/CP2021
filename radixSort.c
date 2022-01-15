#include "radixSort.h"

int getMax(int arr[], int n) {
    int max = arr[0];
    for (int i = 1; i < n; i++) {
        max = arr[i] > max ? arr[i] : max;
    }
    return max;
}

void countSort(int arr[], int n, int e) {
    int count[10] = {0};
    int output[n];
    int i;

    // Fill count array
    for(i = 0; i < n; i++) {
        int digit = (arr[i] / e) % 10;
        count[digit]++;
    }

    // Get positons of numbers in array
    for(i = 1; i < 10; i++) {
        count[i] += count[i-1];
    }

    // Fill output array according to counting array
    for(i = n - 1; i >= 0; i--) {
        int digit = (arr[i] / e) % 10;
        int position = (count[digit]--) - 1;
        output[position] = arr[i];
    }

    // Update previous array with the sorted one
    for (i = 0; i < n; i++)
        arr[i] = output[i];
}

void radixSort(int * arr, int n) {
    // Get max number
    int max = getMax(arr, n); 

    // Sort by digit position
    for (int e = 1; max / e > 0; e *= 10)
    {  
        countSort(arr,n,e);
    }
}