#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define PLUS 0
#define MULT 1 
#define CONC 2

#define IS_TEST false

typedef struct {
  long long total;
  int index; 
  bool active;
} Node; 

long long concat(long long t, int num){
  char buf1[100];
  snprintf(buf1, sizeof(buf1), "%lld", t);
  char buf2[30];
  snprintf(buf2, sizeof(buf2), "%d", num);
  strcat(buf1, buf2);
  char * _endptr;
  return strtoll(buf1, &_endptr, 10); 
}

bool is_plausible(int * numbers, int size, long long target, int mem){
  Node stack[mem];
  int pointer = 0;
  stack[pointer++] = (Node){ .total=numbers[0], .index=1, .active=true};

  while (pointer != 0){
    Node node = stack[--pointer];
    if (node.index == size){
      //leaf node
      if (node.total == target) return true; 
      continue; 
    }
    if (!node.active) continue;
    int p = pointer;
    for (int i = 0; i < 3; i++){
      long long current_total = 0;
      if (i == PLUS){
        current_total = node.total + numbers[node.index]; 
      }else if (i == MULT){
        current_total = node.total * numbers[node.index]; 
      }else if (i == CONC){
        current_total = concat(node.total, numbers[node.index]);
      }else{
        //assert failed.
        return false;
      }
      //printf("current total: %lld, index: %d\n", current_total, node.index+1);
      stack[pointer++] = (Node){ .total=current_total, .index=node.index+1, .active=true};
    }
  }
  return false;
}

void test_is_plausible(void){
  int mem = 1000;
  int test_case1[] = {10,19}; int target1 = 190; int size1 = 2; bool expected1 = true; 
  int test_case2[] = {81,40,27}; int target2 = 3267; int size2 = 3; bool expected2 = true; 
  int test_case3[] = {17,5}; int target3 = 83; int size3 = 2;  bool expected3 = false;
  int test_case4[] = {15,6}; int target4 = 156; int size4 = 2; bool expected4 = false; 
  int test_case5[] = {6,8,6,15}; int target5 = 7290; int size5 = 4; bool expected5 = false; 
  int test_case6[] = {16,10,13}; int target6 = 161011; int size6 = 3; bool expected6 = false; 
  int test_case7[] = {17,8,14}; int target7 = 192; int size7 = 3; bool expected7 = false; 
  int test_case8[] = {9,7,18,13}; int target8 = 21037; int size8 = 4; bool expected8 = false; 
  int test_case9[] = {11,6,16,20}; int target9 = 292; int size9 = 4; bool expected9 = true; 
  
  printf("testing %d\n", 1);
bool actual1 = is_plausible(test_case1, size1, target1, mem);

printf("testing %d\n", 2);
bool actual2 = is_plausible(test_case2, size2, target2, mem);

printf("testing %d\n", 3);
bool actual3 = is_plausible(test_case3, size3, target3, mem);

printf("testing %d\n", 4);
bool actual4 = is_plausible(test_case4, size4, target4, mem);

printf("testing %d\n", 5);
bool actual5 = is_plausible(test_case5, size5, target5, mem);

printf("testing %d\n", 6);
bool actual6 = is_plausible(test_case6, size6, target6, mem);

printf("testing %d\n", 7);
bool actual7 = is_plausible(test_case7, size7, target7, mem);

printf("testing %d\n", 8);
bool actual8 = is_plausible(test_case8, size8, target8, mem);

printf("testing %d\n", 9);
bool actual9 = is_plausible(test_case9, size9, target9, mem);

  assert(actual1 == expected1);
  assert(actual2 == expected2);
  assert(actual3 == expected3);
  assert(actual4 == expected4);
  assert(actual5 == expected5);
  assert(actual6 == expected6);
  assert(actual7 == expected7);
  assert(actual8 == expected8);
  assert(actual9 == expected9);
  
  printf("[TEST is_plausible]: Passed\n");
}

long long process_file1(char * filename, int mem){
  int count = 0;
  FILE * file = fopen(filename, "r");
  char line[256];
  if (file == NULL){
    printf("[ERROR]: Cannot read file '%s'\n", filename);
    return -1;
  }
  long long sum = 0;
  while (fgets(line, sizeof(line), file)){
    int line_size = strlen(line);
    char buffer[100]; int bi = 0; int spacer = 0; 
    for (int i = 0; i < line_size; i++){
      buffer[bi++] = line[i]; 
      if (line[i] == ':'){ spacer = i; buffer[bi++] = '\0'; break; }
    }
    char * endptr;
    long long target = strtoll(buffer, &endptr, 10);
    bi = 0;
    for (int i = spacer+1; i < line_size; i++){
      if (line[i] == '\n') break; 
      buffer[bi++] = line[i];
    }
    buffer[bi++] = '\0';
     char *token = strtok(buffer, " ");
     int numbers[100];
     int numbers_size = 0;
     while (token != NULL) {
        // Convert to integer (if needed)
        numbers[numbers_size++] = atoi(token);
        // Get next token
        token = strtok(NULL, " ");
     }
     int * numbers_ptr = numbers;
       count++;

       printf("target: %lld \t\t [", target);
       for (int i = 0; i < numbers_size; i++){
         printf(" %d ", numbers[i]);
       }
       printf("]\n");

     if (is_plausible(numbers_ptr, numbers_size, target, mem)){
       sum += target;
       
       //       printf("sum: %lld count: %d \t target: %lld \n", sum, count, target);
     }else{
       //printf("target: %lld buffer: %s\n", target, buffer);
       
     }
  }
  fclose(file);
  return sum; 
}

void test_process_file1(void){
  int expected = 3749;
  int actual = process_file1("day7_test.txt", 1000);
  printf("expected: %d, actual: %d\n", expected, actual);
  assert(expected == actual);
  printf("[TEST process_file1]: Passed\n");
}

int main(void){
  if (IS_TEST){
    test_is_plausible();
    test_process_file1();
    return 0;
  }
    printf("result: %lld\n", process_file1("day7_act.txt", 4500));

  return 0; 
}
