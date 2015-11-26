#include <stdio.h>
#include "matrix_strassen.h"

#define SIZE 4

void matrix_strassen_iter(int (*source1)[], int (*source2)[],
                          int (*destination)[], int size);
void matrix_strassen(int (*source)[], int lower_bound, int high_bound);
void matrix_add(int (*source1)[], int (*source2)[], int (*destination)[],
                int low_bound1, int high_bound1, int lower_bound2,
                int high_bound2);
void matrix_sub(int (*source)[], int (*destination)[], int size, int low_bound,
                int high_bound);

int main() {
  FILE *file1;
  int a[SIZE][SIZE];
  int b[SIZE][SIZE];
  int c[SIZE][SIZE];
  int i, j;

  file1 = fopen("source.txt", "r");
  for (i = 0; i < SIZE; i++) {
    for (j = 0; j < SIZE; j++) {
      fscanf(file1, "%d", &a[i][j]);
    }
  }
  fclose(file1);

  matrix_strassen_iter(a, b, c, SIZE);

  for (i = 0; i < SIZE; i++) {
    for (j = 0; j < SIZE; j++) {
      printf("%d", a[i][j]);
    }
    printf("\n");
  }
  /*
      for(i=0;i<3;i++)
      {
          for(j=0;j<3;j++)
          {
              printf("%d ",b[i][j]);
          }
          printf("\n");
      }
  */

  return 0;
}

void matrix_strassen_iter(int (*source1)[], int (*source2)[],
                          int (*destination)[], int size) {
  int mid = size / 2;
  int left = size - mid;
  int *ptr_source1 = (int *)source1, *ptr_source2 = (int *)source2,
      *ptr_destination = (int *)destination;
  int *ptr_matrix1, *ptr_matrix2, *ptr_matrix3, *ptr_matrix4, *ptr_matrix5;

  if (size == 1) {
    *ptr_destination = (*ptr_source1) * (*ptr_source2);
  }

  else {
  }
}

void matrix_add(int (*source1)[], int (*source2)[], int (*destination)[],
                int low_bound1, int high_bound1, int low_bound2,
                int high_bound2) {
  int i, j, k;
  int *ptr_source1, *ptr_source2, *ptr_destination;
  int size;

  if ((high_bound1 - low_bound1) != (high_bound2 - low_bound2)) exit(1);

  size = high_bound1 - low_bound1 + 1;
  size = size * size;

  ptr_source1 = (int *)source1;
  ptr_source2 = (int *)source2;
  ptr_destination = (int *)destination;

  for (i = low_bound1, j = low_bound2, k = 0; i < size; i++, j++, k++)
    ptr_destination[k] = ptr_source1[i] + ptr_source2[j];
}

void matrix_sub(int (*source)[], int (*destination)[], int size, int low_bound,
                int high_bound) {
  int i, j, k, l;
  int *ptr_source = (int *)source;
  int *ptr_destination = (int *)destination;
  int bound = high_bound - low_bound + 1;

  for (i = low_bound, j = 0; i < high_bound + 1; i++, j++) {
    for (k = low_bound, l = 0; k < high_bound + 1; k++, l++)
      *(ptr_destination + j * bound + l) = *(ptr_source + size * i + k);
  }
}
