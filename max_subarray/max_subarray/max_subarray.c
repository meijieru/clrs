#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define SIZE 20

typedef struct {
  int low_index;
  int high_index;
  int sum;
} Max_Subarray_result;

void Max_Cross_Subarray(int *source, Max_Subarray_result *result, int low_index,
                        int mid_index, int high_index);
void Max_Subarray(int *source, Max_Subarray_result *result, int low_index,
                  int high_index);

int main() {
  int source[SIZE];
  FILE *file1;
  int i;
  Max_Subarray_result result;

  file1 = fopen("./source.txt", "r");
  for (i = 0; i < SIZE; i++) fscanf(file1, "%d", &source[i]);  // notice the
                                                               // type
  fclose(file1);
  Max_Subarray(source, &result, 0, SIZE - 1);
  printf("%d\n%d\n%d\n", result.low_index, result.high_index, result.sum);
  return 0;
}

void Max_Subarray2(int *source, Max_Subarray_result *result, int low_index,
                   int high_index) {
  int i;
  int max_sum, sum;
  int left_bound, right_bound;

  sum = 0;
  max_sum = 0;
  for (i = low_index; i < high_index + 1; i++) {
    sum += source[i];

    if (sum > max_sum) max_sum = sum;
  }
  nt maxsum(int a[n])
}

/**
 * @brief nlgn
 *
 * @param source
 * @param result
 * @param low_index
 * @param high_index
 */
void Max_Subarray(int *source, Max_Subarray_result *result, int low_index,
                  int high_index) {
  int mid_index;
  Max_Subarray_result left_result, right_result, cross_result;
  if (low_index == high_index) {
    result->low_index = low_index;
    result->high_index = high_index;
    result->sum = source[low_index];
  } else {
    mid_index = (low_index + high_index) / 2;
    Max_Subarray(source, &left_result, low_index, mid_index);
    Max_Subarray(source, &right_result, mid_index + 1, high_index);
    Max_Cross_Subarray(source, &cross_result, low_index, mid_index, high_index);

    if ((left_result.sum >= right_result.sum) &&
        left_result.sum >= cross_result.sum) {
      result->low_index = left_result.low_index;
      result->high_index = left_result.high_index;
      result->sum = left_result.sum;
    } else if ((right_result.sum >= left_result.sum) &&
               (right_result.sum >= cross_result.sum)) {
      result->low_index = right_result.low_index;
      result->high_index = right_result.high_index;
      result->sum = right_result.sum;
    } else {
      result->low_index = cross_result.low_index;
      result->high_index = cross_result.high_index;
      result->sum = cross_result.sum;
    }
  }
}

/**
 * @brief
 *
 * @param source
 * @param result
 * @param low_index
 * @param mid_index
 * @param high_index
 */
void Max_Cross_Subarray(int *source, Max_Subarray_result *result, int low_index,
                        int mid_index, int high_index) {
  int left_sum = 0, right_sum = 0;
  int sum, max_left, max_right;
  int i;

  sum = 0;

  i = mid_index;
  while ((i >= low_index) && (source[i] >= 0)) {
    left_sum += source[i];
    max_left = i;
    i--;
  };

  i = mid_index + 1;
  while ((i <= high_index) && (source[i] >= 0)) {
    right_sum += source[i];
    max_right = i;
    i++;
  };

  sum = left_sum + right_sum;
  result->low_index = max_left;
  result->high_index = max_right;
  result->sum = sum;
}
