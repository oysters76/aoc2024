#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#define DO_TESTS false

typedef struct {
  int count; 
  int forward[50]; 
} Rule;

Rule rules[100]; 

void add_rule(int x, int y){
  rules[x].forward[rules[x].count++] = y;
}

bool is_correct(int * pages, int size){
  for (int i = 0; i < size; i++){
    int p = pages[i];
    Rule r = rules[p];
    if (r.count < 1) continue;
    for (int j = 0; j < size; j++){
      for (int f = 0; f < r.count; f++){
        if (r.forward[f] == pages[j] && i > j){
          return false;
        }
      }
    }
  }
  return true;
}

void print_int_arr(int * pages, int size){
  printf("[");
  for (int i = 0; i < size; i++)
    printf(" %d ", pages[i]);
  printf("]\n");
}
int find_middle(int * pages, int size){
  if ((size%2) == 0){
    print_int_arr(pages,size); 
  }
  assert((size % 2) != 0); //cannot process even sizes
  int c = size-1;
  return pages[c/2];
}

void test_is_correct(void){
  add_rule(47,53);
  add_rule(97,13); 
  add_rule(97,61);
  add_rule(97,47);
  add_rule(75,29);
  add_rule(61,13); 
  add_rule(75,53);
  add_rule(29,13);
  add_rule(97,29);
  add_rule(53,29);
  add_rule(61,53);
  add_rule(97,53);
  add_rule(61,29);
  add_rule(47,13);
  add_rule(75,47);
  add_rule(97,75);
  add_rule(47,61);
  add_rule(75,61);
  add_rule(47,29);
  add_rule(75,13);
  add_rule(53,13);

  int pages1[] = {75,47,61,53,29}; bool expected1 = true; 
  int pages2[] = {97,61,53,29,13}; bool expected2 = true; 
  int pages3[] = {75,29,13}; bool expected3 = true; 
  int pages4[] = {75,97,47,61,53}; bool expected4 = false; 
  int pages5[] = {61,13,29}; bool expected5 = false; 
  int pages6[] = {97,13,75,29,47}; bool expected6 = false;

  assert(is_correct(pages1, 5) == expected1);
  assert(is_correct(pages2, 5) == expected2);
  assert(is_correct(pages3, 3) == expected3);
  assert(is_correct(pages4, 5) == expected4);
  assert(is_correct(pages5, 3) == expected5);
  assert(is_correct(pages6, 5) == expected6); 
  
  printf("[TEST: test_is_correct]: PASSED\n");
}

void test_find_middle(void){
  int pages1[] = {75,47,61,53,29}; 
  int pages2[] = {97,61,53,29,13}; 
  int pages3[] = {75,29,13};

  assert(find_middle(pages1, 5) == 61);
  assert(find_middle(pages2, 5) == 53);
  assert(find_middle(pages3, 3) == 29);
  printf("[TEST: test_find_middle]: PASSED\n");
}

bool is_rule_line(char * line){
  int size = strlen(line);
  for (int i = 0; i < size; i++){
    if (line[i] == '|') return true; 
  }
  return false;
}

void process_rule_line(char * line){
  line[strcspn(line, "\n")] = '\0';
  int x, y;
  if (sscanf(line, "%d|%d", &x, &y) == 2){
    add_rule(x,y);
    printf("[LOG]: Add %d|%d to rules\n", x,y);
  }else{
    printf("[WARN]: Failed to read rule: %s\n", line);
  }
}

int process_array_line(char * line){
  line[strcspn(line, "\n")] = '\0';
  int pages[100];
  int count = 0;
  char * token = strtok(line, ","); 
  while (token != NULL && count < 100){
    pages[count++] = atoi(token);
    token = strtok(NULL, ","); 
  }
  if (is_correct(pages, count)) {
    return find_middle(pages, count); 
  }
  return -1;
}

int process_file(char * filename){
  FILE * file = fopen(filename, "r");
  char line[256];
  if (file == NULL){
    printf("[ERROR]: Cannot read file '%s'\n", filename);
    return -1;
  }
  int sum = 0;
  while (fgets(line, sizeof(line), file)){
    if (strlen(line) == 1) continue;
    if (is_rule_line(line)){
      process_rule_line(line); 
    }else{
      int result = process_array_line(line);
      if (result <= 0) continue;
      sum += result;
    }
  }
  fclose(file);
  return sum;
}

int compare(const void *a, const void *b){
  int a_i = *(int *)a;
  int b_i = *(int *)b;
  Rule ra = rules[a_i];
  Rule rb = rules[b_i];
  if (ra.count >= 1){
    for (int i = 0; i < ra.count; i++){
      if (ra.forward[i] == b_i){
        return -1; //b is larger
      }
    }
  }
  if (rb.count >= 1){
    for (int i = 0; i < rb.count; i++){
      if (rb.forward[i] == a_i){
        return 1; //a is larger
      }
    }
  }
  return 0;
}

int process_array_line2(char * line){
  line[strcspn(line, "\n")] = '\0';
  int pages[100];
  int count = 0;
  char * token = strtok(line, ","); 
  while (token != NULL && count < 100){
    pages[count++] = atoi(token);
    token = strtok(NULL, ","); 
  }
  if (!is_correct(pages, count)){
    //implement a sort function to sort pages based on rules
    qsort(pages, count, sizeof(int), compare);
    return find_middle(pages, count); 
  }
  return -1;
}

int process_file2(char * filename){
  FILE * file = fopen(filename, "r");
  char line[256];
  if (file == NULL){
    printf("[ERROR]: Cannot read file '%s'\n", filename);
    return -1;
  }
  int sum = 0;
  while (fgets(line, sizeof(line), file)){
    if (strlen(line) == 1) continue;
    if (is_rule_line(line)){
      process_rule_line(line); 
    }else{
      int result = process_array_line2(line);
      if (result <= 0) continue;
      sum += result;
    }
  }
  fclose(file);
  return sum;
}

void test_process_file(void){
  int actual = process_file("test_input_day5.txt"); 
  int expected = 143;
  assert(actual == expected);
  printf("[TEST test_process_file]: Passed\n");
}

void test_process_file2(void){
  int actual = process_file2("test_input_day5.txt"); 
  int expected = 123;
  assert(actual == expected);
  printf("[TEST test_process_file2]: Passed\n");
}

int main(void){
  if (DO_TESTS){
    test_process_file2();
    return 0;
  }
  int result = process_file2("day5_input.txt");
  printf("result: %d\n", result);

  return 0;
}
