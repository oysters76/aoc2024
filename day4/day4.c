#include <stdio.h>
#include <string.h> 
#include <stdbool.h>
#include <assert.h>


#define IS_TEST false

const char * MATCH_STR = "XMAS";
const int MATCH_STR_LEN = 4; 

int find_matches(int w, int h, char grid[][w]); 
int find_matches_horizontal(int w, int h, char grid[][w]);
int find_matches_vertical(int w, int h, char grid[][w]);
int find_matches_diag(int w, int h, char grid[][w]); 
bool is_match(char word[MATCH_STR_LEN]);

int find_matches(int width, int height, char grid[][width]){
  printf("at find_matches\n");
  return find_matches_horizontal(width, height, grid) +
    find_matches_vertical(width, height, grid) +
    find_matches_diag(width, height, grid); 
}

void test_is_match(void){
  char test1[] = {'X', 'M', 'A', 'S'};
  char test2[] = {'X', 'X', 'A', 'M'};
  char test3[] = {'S', 'A', 'M', 'X'};

  assert(is_match(test1) == true);
  assert(is_match(test2) == false);
  assert(is_match(test3) == true);
  printf("[TEST test_is_match]: Passed\n");
}

bool not_valid_pos(int w, int h, int * pos){
  int y = pos[0];
  int x = pos[1];
  return !((x >= 0 && x < w) && (y >= 0 && y < h));
}



void print_mas(char * mas){
  for (int i = 0; i < 3; i++)
    printf("%c",mas[i]);
}

bool valid_mas(char * mas){
  char c1 = mas[0];
  char c2 = mas[1];
  char c3 = mas[2];

  if (c1 == 'M' && c2 == 'A' && c3 == 'S') return true;
  if (c1 == 'S' && c2 == 'A' && c3 == 'M') return true;
  return false;
}


int find_grid_matches(int w, int h, char grid[][w]){
  int count = 0; 
  for (int i = 0; i < h; i++){
    for (int j = 0; j < w; j++){
      int pos_1[] = {i,j};
      int pos_2[] = {i,j+2};
      int pos_3[] = {i+1,j+1};
      int pos_4[] = {i+2,j};
      int pos_5[] = {i+2,j+2};
      if (not_valid_pos(w, h, pos_1) || not_valid_pos(w, h, pos_2) || not_valid_pos(w, h, pos_3) || not_valid_pos(w, h, pos_4) || not_valid_pos(w, h, pos_5)){
        continue; 
      }
      char c1 = grid[pos_1[0]][pos_1[1]];
      char c2 = grid[pos_2[0]][pos_2[1]];
      char c3 = grid[pos_3[0]][pos_3[1]];
      char c4 = grid[pos_4[0]][pos_4[1]];
      char c5 = grid[pos_5[0]][pos_5[1]];
      char mas1 [] = {c1, c3, c5};
      char mas2 [] = {c2, c3, c4};
      if (valid_mas(mas1) && valid_mas(mas2)) count++;
    }
  }
  return count;
}

void test_find_matches(void){
  int w = 10;
  int h = 10;
  char grid[h][w]; // VLA
  char temp[10][10] = {
    {'M','M','M','S','X','X','M','A','S','M'},
    {'M','S','A','M','X','M','S','M','S','A'},
    {'A','M','X','S','X','M','A','A','M','M'},
    {'M','S','A','M','A','S','M','S','M','X'},
    {'X','M','A','S','A','M','X','A','M','M'},
    {'X','X','A','M','M','X','X','A','M','A'},
    {'S','M','S','M','S','A','S','X','S','S'},
    {'S','A','X','A','M','A','S','A','A','A'},
    {'M','A','M','M','M','X','M','M','M','M'},
    {'M','X','M','X','A','X','M','A','S','X'}
  };
  for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            grid[i][j] = temp[i][j];
        }
    }
  int expected = 18;
  int actual = find_matches(w, h, grid);
  printf("actual: %d expected: %d\n", actual, expected);
  assert(expected == actual);
  printf("[TEST test_find_matches]: Passed\n"); 
}

void test_sliding_grid(void){
  int w = 10;
  int h = 10;
  char grid[h][w]; // VLA
  char temp[10][10] = {
    {'M','M','M','S','X','X','M','A','S','M'},
    {'M','S','A','M','X','M','S','M','S','A'},
    {'A','M','X','S','X','M','A','A','M','M'},
    {'M','S','A','M','A','S','M','S','M','X'},
    {'X','M','A','S','A','M','X','A','M','M'},
    {'X','X','A','M','M','X','X','A','M','A'},
    {'S','M','S','M','S','A','S','X','S','S'},
    {'S','A','X','A','M','A','S','A','A','A'},
    {'M','A','M','M','M','X','M','M','M','M'},
    {'M','X','M','X','A','X','M','A','S','X'}
  };
  for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            grid[i][j] = temp[i][j];
        }
    }
  int expected = 9;
  int actual = find_grid_matches(w, h, grid);
  printf("actual: %d expected: %d\n", actual, expected);
  assert(expected == actual);
  printf("[TEST test_find_grid_matches]: Passed\n");   
}

void print_sliding_window(char * arr){
  printf("["); 
  for (int i = 0; i < MATCH_STR_LEN; i++)
    printf("%c", arr[i]);
  printf("] --- ");
  printf("[");
  for (int i = MATCH_STR_LEN-1; i >= 0; i--)
    printf("%c", arr[i]);
  printf("] = %s\n", is_match(arr) ? "YES!" : "NO");
}

int find_matches_diag(int w, int h, char grid[][w]){
  int count = 0;
  const int word_width = MATCH_STR_LEN;
  for (int i = 0; i <= (h-word_width); i++){
    char sliding_window[word_width]; 
    for (int j = 0; j <= (w-word_width); j++){
      for (int k = 0; k < word_width; k++){
        sliding_window[k] = grid[i+k][j+k];  
      }
      print_sliding_window(sliding_window);
      if (is_match(sliding_window)) count++;
    }
    for (int j = w-1; j >= (word_width-1); j--){
      for (int k = 0; k < word_width; k++){
        sliding_window[k] = grid[i+k][j-k]; 
      }
      print_sliding_window(sliding_window); 
      if (is_match(sliding_window)) count++;
    }
  }
  printf("diag count: %d\n", count);
  return count; 
}

int find_matches_vertical(int w, int h, char grid[][w]){
  int count = 0; 
  const int word_width = MATCH_STR_LEN; 
  for (int i = 0; i < w; i++){
    for (int j = 0; j <= (h-word_width); j++){
      char sliding_window[word_width]; 
      for (int k = 0; k < word_width; k++){
        sliding_window[k] = grid[j+k][i];
      }
      print_sliding_window(sliding_window);
      if (is_match(sliding_window)) {
        printf("pos: %d, %d\n", i, j);
        count++;
      }
    }
  }
  printf("vertical count: %d\n", count);
  return count; 
}

int find_matches_horizontal(int w, int h, char grid[][w]){
  int count = 0; 
  const int word_width = MATCH_STR_LEN; 
  for (int i = 0; i < h; i++){
    for (int j = 0; j <= (w-word_width); j++){
      char sliding_window[word_width]; 
      for (int k = 0; k < word_width; k++){
        sliding_window[k] = grid[i][j+k]; 
      }
      if (is_match(sliding_window)) count++; 
    }
  }
  printf("horizontal count: %d\n", count); 
  return count; 
}

bool is_match(char word[MATCH_STR_LEN]){
  bool is_a_match = true; 
  for (int i = 0; i < MATCH_STR_LEN; i++){
    char w = word[i];
    if (w != MATCH_STR[i]){
      is_a_match = false;
      break;
    }
  }
  if (is_a_match) return true;
  int j = 0;
  for (int i = MATCH_STR_LEN-1; i >= 0; i--){
    char w = word[j++];
    if (w != MATCH_STR[i]){
      return false; 
    }
  }
  return true; 
}

int main(void){

  if (IS_TEST){
    test_sliding_grid();
    return 0;
  }
  
  FILE *file = fopen("day4_input.txt", "r");
  if (file == NULL){
    printf("couldn't open file\n");
    return 1; 
  }
  char line[1000];
  int h = 0; int w = 0;
  while (fgets(line, 1000, file) != NULL){
    w = strlen(line);
    h++;
  }
  printf("grid width: %d, height: %d\n", w, h);
  fclose(file);

  char grid[h][w-1];
  int h_i = 0;
  file = fopen("day4_input.txt", "r");
  if (file == NULL){
    printf("couldn't open file\n");
    return 1;
  }
  while (fgets(line, 1000, file) != NULL){
    for (int j = 0; j < (w-1); j++){
      grid[h_i][j] = line[j]; 
    }
    h_i++;
  }
  fclose(file);
  int count = find_matches(w-1, h, grid);
  printf("result: %d\n", count);
  int count2 = find_grid_matches(w-1, h, grid);
  printf("result: %d\n", count2);
  
  
  return 0; 
}
