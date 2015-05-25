#ifndef __MERGE_H
#define __MERGE_H

/* Merge_sort */
#define LAST_ELEMENT INT_MAX 


void Merge_sort_call(int *source,int size);
void Merge_sort(int *source,int left_index,int right_index);
void Merge(int *source,int left_index,int mid_index,int right_index);

#endif // __MERGE_H
