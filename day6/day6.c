#include <stdio.h>
#include <string.h> 
#include <stdbool.h>
#include <assert.h> 
#include <stdlib.h>
#include <pthread.h>

#define IS_TEST false
#define UP 0
#define RIGHT 1
#define BOTTOM 2
#define LEFT 3

bool is_valid(int x, int y, int w, int h){
  return x >= 0 && y >= 0 && x < w && y < h; 
}

typedef struct {
  int x;
  int y; 
} Position; 

typedef struct {
  int x;
  int y;
  int dir; 
} Block;


#define M_WIDTH 200
#define M_HEIGHT 200

int get_all_visited(int w, int h, int dir, int px, int py, char grid[][w], int mem){
  Position visited[mem];
  int visited_count = 0;

  visited[visited_count++] = (Position){ .x = px, .y = py }; 
  int ppx = px;
  int ppy = py; 
  while (is_valid(px, py, w, h)){
    ppx = px;
    ppy = py;
    if (dir == UP){
      py--;
    }else if (dir == RIGHT){
      px++; 
    }else if (dir == BOTTOM){
      py++; 
    }else if (dir == LEFT){
      px--;
    }else{
      return -1; 
    }
    if (!is_valid(px, py, w, h)){
      break; 
    }
    if (grid[py][px] == '#'){
      printf("block: %d, %d\n", px, py);
      px = ppx;
      py = ppy;
      dir = (++dir)%4; 
    }
    bool isVisited = false; 
    for (int i = 0; i < visited_count; i++){
      if (visited[i].x == px && visited[i].y == py){
        isVisited = true; break; 
      }
    }
    if (!isVisited){
      visited[visited_count++] = (Position){ .x = px, .y = py}; 
    }
  }

  printf("\n");
  for (int i = 0; i < h; i++){
    printf("[");
    for (int j = 0; j < w; j++){
      bool isVisit = false; 
      for (int k = 0; k < visited_count; k++){
        Position p = visited[k];
        if (p.x == j && p.y == i){
          printf("%c", 'X');
          isVisit = true; 
          break; 
        }
      }
      if (isVisit) continue; 
      printf("%c", grid[i][j]);
    }
    printf("]\n"); 
  }
  
  return visited_count; 
}

int is_cycle(int w, int h, int dir, int px, int py, char grid[][w], int mem){
  Block blocks[mem];
  int blocks_count = 0;

  int ppx = px;
  int ppy = py;
  while (is_valid(px, py, w, h)){
    ppx = px;
    ppy = py;
    if (dir == UP){
      py--;
    }else if (dir == RIGHT){
      px++; 
    }else if (dir == BOTTOM){
      py++; 
    }else if (dir == LEFT){
      px--;
    }else{
      return -1; 
    }
    if (!is_valid(px, py, w, h)){
      break; 
    }
    if (grid[py][px] == '#'){
      blocks[blocks_count++] = (Block){ .x = px, .y = py, .dir = dir }; 
      px = ppx;
      py = ppy;
      dir = (++dir)%4;
    }
    for (int b = 0; b < blocks_count; b++){
      Block p1 = blocks[b];
      for (int k = 0; k < blocks_count; k++){
        if (b == k) continue;
        Block p2 = blocks[k];
        if (p1.x == p2.x && p1.y == p2.y && p1.dir == p2.dir){
          return true;
        }
      }
    }
  }
  return false;
}

int get_total_cycles(int w, int h, int dir, int px, int py, char grid[][w], int mem){
  int count = 0;
  float total = (float) w*h;
  float completed = 0.0f;
  
  for (int i = 0; i < h; i++){
    for (int j = 0; j < w; j++){
      if (px == j && py == i) continue;
      char old_val = grid[i][j]; 
      if (old_val == '#') continue; 
      grid[i][j] = '#';
      if (is_cycle(w,h,dir,px,py,grid,mem)){
        count++;
      }
      grid[i][j] = old_val;
      completed+=1.0f;
      printf("\rdone (%d,%d), progress: %.2f ( %.2f out of %f )", i,j, (completed/total)*100, completed, total);
    }
  }
  return count;
}

int process_file(char * filename, int mem){
  FILE *file = fopen(filename, "r");
  char line[256];
  int w = 0; int h = 0;
  if (file == NULL){ return -1; }
  while (fgets(line, sizeof(line), file)){
    line[strcspn(line, "\n")] = '\0';
    w = strlen(line);
    h++; 
  }
  fclose(file);
  
  char grid[h][w];  int h_i = 0;
  int px, py;
  int dir = UP;

  file = fopen(filename, "r");
  if (file == NULL) { return -1; }
  while (fgets(line, sizeof(line), file)){
    line[strcspn(line, "\n")] = '\0';
    for (int i = 0; i < w; i++)
      {
        grid[h_i][i] = line[i];
        if (line[i] == '^'){
          px = i;
          py = h_i; 
          grid[h_i][i] = '.'; 
        }
      }
    h_i++;
  }
  fclose(file);

  return get_all_visited(w, h, UP, px, py, grid, mem); 
}

int process_file2(char * filename, int mem){
  FILE *file = fopen(filename, "r");
  char line[256];
  int w = 0; int h = 0;
  if (file ==  NULL){ return -1; }
  while (fgets(line, sizeof(line), file)){
    line[strcspn(line, "\n")] = '\0';
    w = strlen(line);
    h++; 
  }
  fclose(file);
  
  char grid[h][w];  int h_i = 0;
  int px, py;
  int dir = UP;

  file = fopen(filename, "r");
  if (file == NULL) { return -1; }
  while (fgets(line, sizeof(line), file)){
    line[strcspn(line, "\n")] = '\0';
    for (int i = 0; i < w; i++)
      {
        grid[h_i][i] = line[i];
        if (line[i] == '^'){
          px = i;
          py = h_i; 
          grid[h_i][i] = '.'; 
        }
      }
    h_i++;
  }
  fclose(file);

  printf("size: %d, %d, total: %d\n", w, h, w*h);
  return get_total_cycles(w, h, dir, px, py, grid, mem);
}


void test_process_file(void){
  int expected = 41;
  int actual = process_file("day6_test.txt", 100);
  printf("expected: %d, actual: %d\n", expected, actual);
  assert(expected == actual);
  printf("[TEST: test_process_file]: Passed\n"); 

  expected = 6;
  actual = process_file2("day6_test.txt", 100);
  printf("expected: %d, actual: %d\n", expected, actual);
  assert(expected == actual);
  printf("[TEST: test_process_file2]: Passed\n");
}


int main(void){
  if (IS_TEST){
  test_process_file();
  return 0; 
  }
  int result = process_file2("day6_act.txt", 25000);
  printf("result:%d\n", result);
  return 0; 
}
