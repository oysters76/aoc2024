#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define BUF_SIZE 100000
#define MAX_CODE_LENGTH 12

const char VALID_CODE_CH [] = {'m', 'u', 'l', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '(' , ')', ','}; 
const int VALID_CODE_CH_SIZE = 16;
  
bool is_valid_ch(char ch){
  for (int i = 0; i < VALID_CODE_CH_SIZE; i++){
    if (ch == VALID_CODE_CH[i]){
      return true; 
    }
  }
  return false;
}

void find_instr_positions(char * code, int code_size, char * instr_str, int * result_arr, int * result_arr_size){
  int instr_str_len = strlen(instr_str); 
  for (int i = 0; i < code_size-instr_str_len; i++){
    char partition[instr_str_len]; 
    for (int k = 0; k < instr_str_len; k++){
      partition[k] = code[i+k]; 
    }
    bool is_match = true; 
    for (int k = 0; k < instr_str_len; k++){
      if (instr_str[k] != partition[k]) {
        is_match = false;
        break;
      }
    }
    if (!is_match) continue;
    int write_index = *result_arr_size; 
    result_arr[write_index] = i;
    *result_arr_size = ++write_index; 
  }
}

int main(void){
  FILE * file = fopen("day3_input.txt", "r");

  if (file == NULL){
    printf("Error opening file\n");
    return 1; 
  }

  int ch;
  char code[BUF_SIZE];
  int code_size = 0; 

  while ((ch = fgetc(file)) != EOF){
    code[code_size++] = ch; 
  }

  int code_map[code_size]; //maps mul results & do & donts into an indexed array
  for (int k = 0; k < code_size; k++){
    code_map[k] = -10; 
  }
  
  int mult_indexes[BUF_SIZE/100];
  int mult_indexes_size = 0; 

  int program_output = 0; 
  
  for (int i = 0; i < code_size-3; i++){
    char c1 = code[i];
    char c2 = code[i+1];
    char c3 = code[i+2];
    if (c1 == 'm' && c2 == 'u' && c3 == 'l'){
      mult_indexes[mult_indexes_size++] = i; 
    }
  }

  char code_segment_buffer[BUF_SIZE/1000]; 
  int code_segment_buffer_size = 0; 

  int filtered_mul_indexes[BUF_SIZE/100];
  int filtered_mul_indexes_size = 0;
  
  for (int i = 0; i < mult_indexes_size; i++){
    code_segment_buffer_size = 0; 
    int position = mult_indexes[i];
    char ch = code[position];
    while (ch != ')' && position < code_size){
      code_segment_buffer[code_segment_buffer_size++] = ch; 
      ch = code[++position]; 
    }
    code_segment_buffer[code_segment_buffer_size++] = ')';
    code_segment_buffer[code_segment_buffer_size++] = '\0';
    bool is_valid = true; 
    int segment_length = strlen(code_segment_buffer); 
    if (segment_length > MAX_CODE_LENGTH) is_valid = false;
    
    if (!is_valid) continue;

    //printf("%s\n", code_segment_buffer); 
    
    for (int k = 0; k < segment_length; k++){
      char code_ch = code_segment_buffer[k];
      if (!is_valid_ch(code_ch)){
        is_valid = false;
        break; 
      }
    }

    if (!is_valid) continue; 

    int i1 = 0;
    int i2 = 0;
    if (sscanf(code_segment_buffer, "mul(%d,%d)", &i1, &i2) == 2){
      //printf("code: %s, %d x %d = %d\n", code_segment_buffer, i1, i2, i1 * i2); 
      program_output += (i1 * i2); 
      filtered_mul_indexes[filtered_mul_indexes_size++] = mult_indexes[i]; 
      code_map[mult_indexes[i]] = (i1 * i2); 
    }else{
      //printf("-> rejected %s\n", code_segment_buffer); 
    }
    
  }

  //printf("-------------------------\n");
  printf("result: %d\n", program_output); 

  char * do_str = "do()";
  char * dont_str = "don't()"; 
  int do_instr_positions[BUF_SIZE/1000];
  int do_instr_positions_size = 0; 
  int dont_instr_positions[BUF_SIZE/1000]; 
  int dont_instr_positions_size = 0;
  
  find_instr_positions(code, code_size, do_str, do_instr_positions, &do_instr_positions_size); 
  find_instr_positions(code, code_size, dont_str, dont_instr_positions, &dont_instr_positions_size); 
  
  for (int k = 0; k < do_instr_positions_size; k++){
    code_map[do_instr_positions[k]] = -2; //-2 denotes do instr
  }

  for (int k = 0; k < dont_instr_positions_size; k++){
    code_map[dont_instr_positions[k]] = -1; //-1 denotes dont instr 
  }

  bool add_flag = true;
  int result_2 = 0; 
  for (int k = 0; k < code_size; k++){
    int v = code_map[k];
    if (v == -2){
      // printf("[%d]: %s\n", k, "do");
      add_flag = true; 
    }else if (v == -1){
      // printf("[%d]: %s\n", k, "dont");
      add_flag = false; 
    }else if (v > 0){
      if (add_flag){
        result_2 += v; 
      }
      //      printf("[%d]: %d\n", k, v); 
    }
  }
  //printf("---------------------------------\n");
  printf("result2: %d\n", result_2); 
  
  
  fclose(file); 
  return 0; 
}
