#include "quickSort.h"

void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}

int partition(int array[], int low, int high) {
  
  int pivot = array[high];

  int i = (low - 1);

  for (int j = low; j < high; j++) {
    if (array[j] <= pivot) {

      i++;

      swap(&array[i], &array[j]);
    }
  }

  swap(&array[i + 1], &array[high]);

  return (i + 1);
}

void qSort(int array[], int low, int high) {
  if (low < high) {

    int pi = partition(array, low, high);

    qSort(array, low, pi - 1);

    qSort(array, pi + 1, high);
  }
}

void quickSort(int array[], int n) {
    qSort(array,0,n-1);
}