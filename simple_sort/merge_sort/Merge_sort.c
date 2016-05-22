#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "Merge_sort.h"

#define SIZE 20

/**
 * @brief
 *
 * @param source
 * @param size
 */
void Merge_sort_call(int *source, int size) {
  int i;

  Merge_sort(source, 0, size - 1);
#ifndef DEBUG
  for (i = 0; i < size; i++) printf("%d ", source[i]);
#endif
  printf("Merge_sort_call\n");
}

/**
 * @brief
 *
 * @param source
 * @param left_index
 * @param right_index
 */
void Merge_sort(int *source, int left_index, int right_index) {
  int mid_index;
  if (left_index < right_index) {
    mid_index = (left_index + right_index) / 2;
    Merge_sort(source, left_index, mid_index);
    Merge_sort(source, mid_index + 1, right_index);
    Merge(source, left_index, mid_index, right_index);
  }
}

/**
 * @brief
 *
 * @param source
 * @param left_index
 * @param mid_index
 * @param right_index
 */
void Merge(int *source, int left_index, int mid_index, int right_index) {
  int count1, count2, count;
  int i, j, k;
  int *array1, *array2;

  count1 = mid_index - left_index + 1;
  count2 = right_index - mid_index;
  count = count1 + count2;

  array1 = (int *)malloc(sizeof(int) * (count1 + 1));
  if (array1 == NULL) exit(0);
  array2 = (int *)malloc(sizeof(int) * (count2 + 1));
  if (array2 == NULL) exit(0);

  for (i = 0; i < count1; i++) array1[i] = source[left_index + i];
  array1[count1] = LAST_ELEMENT;

  for (i = 0; i < count2; i++) array2[i] = source[mid_index + i + 1];
  array2[count2] = LAST_ELEMENT;

  j = 0;
  k = 0;
  for (i = 0; i < count; i++) {
    if (array1[j] < array2[k]) {
      source[left_index + i] = array1[j];
      j++;
    } else {
      source[left_index + i] = array2[k];
      k++;
    }
  }

  free(array1);
  free(array2);
}
