#ifndef __RANDOM_SEARCH_H
#define __RANDOM_SEARCH_H

int Scramble_search(int *source, int left_index, int right_index, int target);
int Deterministic_search(int *source, int left_index, int right_index,
                         int target);
int Random_search(int *source, int left_index, int right_index, int target);

#endif  // __RANDOM_SEARCH_H
