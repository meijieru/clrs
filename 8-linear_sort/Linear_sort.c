#include <stdio.h>
#include <stdlib.h>

void Counting_sort(int *source, int *destination, int size, int k);
void Radix_sort(int *source, int length);
void Bucket_sort(int *source, int length);
void debug(int *src, int size);

#define SIZE 10
#define ASSUME_LIMIT 100

int main() {
  int i;
  FILE *fin = fopen("random.txt", "r");
  int src[SIZE], dst[SIZE];
  for (i = 0; i < SIZE; i++) fscanf(fin, "%d", &src[i]);
  debug(src, SIZE);
  Counting_sort(src, dst, SIZE, ASSUME_LIMIT);
  debug(dst, SIZE);
}

void Counting_sort(int *source, int *destination, int size, int k) {
  int i;
  int *count = malloc(sizeof(int) * (k + 1));

  if (count == NULL) {
    exit(0);
  }

  for (i = 0; i < k + 1; i++) {
    count[i] = 0;
  }

  for (i = 0; i < size; i++) {
    count[source[i]]++;
  }

  for (i = 1; i < k + 1; i++) {
    count[i] += count[i - 1];
  }

  for (i = size - 1; i >= 0; i--) {
    destination[--count[source[i]]] = source[i];
  }

  free(count);
}

void Radix_sort(int *source, int length) {
  int i;

  for (i = 0; i < length; i++) {
  }
}

/**
 * @brief expect the input are equidistribution
 *
 * @param source
 * @param length
 */
void Bucket_sort(int *source, int length) {}

void debug(int *src, int size) {
  int i;
  for (i = 0; i < size; i++) printf("%d ", src[i]);
  printf("\n");
}
