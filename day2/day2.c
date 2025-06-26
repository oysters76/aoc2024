#include <stdio.h>
#include <string.h> 
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>

#define RUN_TESTS false

bool check_if_safe(int *, int, bool);
bool check_if_safe_if_remove(int *, int); 

bool check_if_safe_if_remove(int * report, int size){
  for (int i = 0; i < size; i++){
    int temp_report[100];
    int temp_report_size = 0; 
    for (int j = 0; j < size; j++){
      if (i == j) continue;
      temp_report[temp_report_size++] = report[j]; 
    }
    bool is_safe = check_if_safe(temp_report, temp_report_size, false);
    if (is_safe) return true;
  }
  return false;
}

void test_check_if_safe_if_remove(void){
  int report_1 [] = {7,6,4,2,1}; bool report_1_result_is_safe = true;
  int report_2 [] = {1,2,7,8,9}; bool report_2_result_is_safe = false;
  int report_3 [] = {9,7,6,2,1}; bool report_3_result_is_safe = false;
  int report_4 [] = {1,3,2,4,5}; bool report_4_result_is_safe = true;
  int report_5 [] = {8,6,4,4,1}; bool report_5_result_is_safe = true;
  int report_6 [] = {1,3,6,7,9}; bool report_6_result_is_safe = true;

  assert(check_if_safe_if_remove(report_1, 5) == report_1_result_is_safe);
  assert(check_if_safe_if_remove(report_2, 5) == report_2_result_is_safe);
  assert(check_if_safe_if_remove(report_3, 5) == report_3_result_is_safe);
  assert(check_if_safe_if_remove(report_4, 5) == report_4_result_is_safe);
  assert(check_if_safe_if_remove(report_5, 5) == report_5_result_is_safe);
  assert(check_if_safe_if_remove(report_6, 5) == report_6_result_is_safe);

  printf("[TEST check_if_safe_if_remove]: PASSED\n"); 
}

bool check_if_safe(int * report, int size, bool debug){
  // return true if all values decrease by 1 or 2 (diff between elements is -1, or -2)
  // return true if all values increase by 1, 2, or 3 (diff between elements is 1,2,3)
  bool is_rule_1 = false;
  bool is_rule_2 = false; 
  for (int i = 0; i < size-1; i++){
    int a = report[i];
    int b = report[i+1];
    int diff = a - b;
    if (debug){
      printf("[DEBUG]: a: %d, b: %d, diff: %d\n", a, b, diff); 
    } 
    bool condition_1 = diff == 1 || diff == 2 || diff == 3;
    bool condition_2 = diff == -1 || diff == -2 || diff == -3; 
    if (i == 0){
      //set rule flags.
      is_rule_1 = condition_1;
      is_rule_2 = condition_2;
      if (is_rule_1 || is_rule_2) continue;
      return false; 
    }
    if (is_rule_1 && condition_1) continue;
    if (is_rule_2 && condition_2) continue;
    return false; 
  }
  return true; 
}

void test_check_if_safe(void){
  int report_1 [] = {7,6,4,2,1}; bool report_1_result_is_safe = true;
  int report_2 [] = {1,2,7,8,9}; bool report_2_result_is_safe = false;
  int report_3 [] = {9,7,6,2,1}; bool report_3_result_is_safe = false; 
  int report_4 [] = {1,3,2,4,5}; bool report_4_result_is_safe = false;
  int report_5 [] = {8,6,4,4,1}; bool report_5_result_is_safe = false;
  int report_6 [] = {1,3,6,7,9}; bool report_6_result_is_safe = true; 

  assert(check_if_safe(report_1, 5, false) == report_1_result_is_safe); 
  assert(check_if_safe(report_2, 5, false) == report_2_result_is_safe);
  assert(check_if_safe(report_3, 5, false) == report_3_result_is_safe);
  assert(check_if_safe(report_4, 5, false) == report_4_result_is_safe);
  assert(check_if_safe(report_5, 5, false) == report_5_result_is_safe);
  assert(check_if_safe(report_6, 5, false) == report_6_result_is_safe);

  printf("[TEST check_if_safe]: PASSED\n"); 
}

int main(void){
  if (RUN_TESTS){
    test_check_if_safe();
    test_check_if_safe_if_remove();
    return 0; 
  }

  FILE * file = fopen("day2_input.txt", "r");
  if (file == NULL){
    perror("Error opening file\n");
    return 1; 
  }

  char buffer[256];
  int total_safe_reports = 0; 
  int total_safe_if_remove_reports = 0; 
  while (fgets(buffer, sizeof(buffer), file) != NULL){
    int size = strlen(buffer);
    char number_buffer[10]; 
    int number_buffer_index = 0;
    int report[100];
    int report_size = 0; 
    for (int i = 0; i < size; i++){
      char c = buffer[i];
      if (c == '\n' || c == ' '){
        number_buffer[number_buffer_index++] = '\0'; 
        number_buffer_index = 0;
        report[report_size++] = atoi(number_buffer); 
      }else{
        number_buffer[number_buffer_index++] = c; 
      }
    }
    bool is_safe = check_if_safe(report, report_size, false);
    if (is_safe) total_safe_reports++;
    
    bool is_safe_if_remove = check_if_safe_if_remove(report, report_size);
    if (is_safe_if_remove) total_safe_if_remove_reports++; 

    printf("\tLine: %s \tis_safe: %s, is_safe_if_remove: %s\n\n", buffer, is_safe ? "Yes" : "No", is_safe_if_remove ? "Yes" : "No");  
  }

  printf("Total safe reports: %d\n", total_safe_reports);
  printf("Total safe if remove reports: %d\n", total_safe_if_remove_reports); 
  fclose(file); 
  return 0; 
}
