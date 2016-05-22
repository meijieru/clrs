#include <stdio.h>
#include "select_sort.h"

static void swap(int *source1, int *source2);
void Select_sort(int *source, int left_index, int right_index);

void Select_sort_call(int *source, int size) {
  int i;

  Select_sort(source, 0, size - 1);
#ifndef DEBUG
  for (i = 0; i < size; i++) printf("%d ", source[i]);
#endif
  printf("Select_sort_call\n");
}

void Select_sort(int *source, int left_index, int right_index) {
  int tmp = source[left_index];
  int tmp_index = left_index;
  int i;

  if (left_index >= right_index) return;

  for (i = left_index + 1; i < right_index + 1; i++) {
    if (source[i] < tmp) {
      tmp = source[i];
      tmp_index = i;
    }
  }
  swap(&source[tmp_index], &source[left_index]);

  Select_sort(source, left_index + 1, right_index);
}

static void swap(int *source1, int *source2) {
  int tmp;
  tmp = *source2;
  *source2 = *source1;
  *source1 = tmp;
}
