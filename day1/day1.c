#include <stdio.h>
#include <stdlib.h>

#define ARR_SIZE 1000

int compare(const void *a, const void *b){
  return (*(int *)a - *(int *)b); 
}

int main(void){
  FILE *file = fopen("in.txt", "r");
  if (file == NULL){
    return 1; 
  }
  char line[256];
  int count = 0;
  int left_arr[ARR_SIZE];
  int right_arr[ARR_SIZE]; 

  while (fgets(line, sizeof(line), file) != NULL){
    int left_num = 0;
    int right_num = 0; 
    sscanf(line, "%d   %d", &left_num, &right_num);
    left_arr[count] = left_num;
    right_arr[count] = right_num;
    count++;
  }
  
  qsort(left_arr, ARR_SIZE, sizeof(int), compare); 
  qsort(right_arr, ARR_SIZE, sizeof(int), compare); 

  int tot_diff = 0; 
  for (int i = 0; i < ARR_SIZE; i++){
    int diff = abs(left_arr[i] - right_arr[i]);
    tot_diff += diff; 
  }
  printf("result 1: %d\n", tot_diff);

  int tot_sim = 0; 
  for (int i = 0; i < ARR_SIZE; i++){
    int instances_found = 0; 
    int left = left_arr[i];
    for (int j = 0; j < ARR_SIZE; j++){
      int right = right_arr[j];
      if (left < right) break;
      if (left == right) instances_found++;
    }
    tot_sim += (instances_found * left_arr[i]); 
  }
  printf("result 2: %d\n", tot_sim); 
  fclose(file); 
  return 0; 
}
