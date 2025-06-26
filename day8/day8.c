#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define IS_TEST false
#define ANTENNAS_HM_SIZE 1000
#define ANTENNAS_INSTANCES 100

typedef struct {
  int x;
  int y; 
} Position;

typedef struct {
  int count;
  Position list[ANTENNAS_INSTANCES];
  bool active;
} Positions; 

Positions ANTENNAS_MAP[ANTENNAS_HM_SIZE]; 

bool in_bounds(int x, int bound){
  return x >= 0 && x < bound; 
}

void init_antennas_positions(void){
  for (int i = 0; i < ANTENNAS_HM_SIZE; i++){
    Positions * p = &ANTENNAS_MAP[i];
    p->count = 0;
    p->active = false;
  }
}

int hm_get_map_index_from_char(char c){
  return (int)c % ANTENNAS_HM_SIZE;
}

int compare_positions(const void *a, const void *b){
  const Position *p1 = (const Position *)a;
  const Position *p2 = (const Position *)b;

  if (p1->x != p2->x){
    return p1->x - p2->x; 
  }
  
  return p1->y - p2->y;
}

void process_line(char * line, int size, int row){
  for (int i = 0; i < size; i++){
    char c = line[i];
    if (c != '.' && c != '\n'){
      Positions * p = &ANTENNAS_MAP[hm_get_map_index_from_char(c)];
      Position new_pos = (Position){ .x=i, .y=row };
      if (p->count < ANTENNAS_INSTANCES){
        printf("found char: %c, position in map is: %d\n", c, hm_get_map_index_from_char(c));
        p->list[p->count] = new_pos;
        p->count++;
        p->active = true;
      }else{
        printf("[ABORT]: too many antennas! limit: %d\n", ANTENNAS_INSTANCES); 
        assert(false); 
      }
    }
  }
}

int process_file(char * filename, bool is_part2){
  init_antennas_positions();
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

  file = fopen(filename, "r");
  if (file == NULL) {return -1; }
  int h_i = 0;
  while (fgets(line, sizeof(line), file)){
    process_line(line, w, h_i);
    h_i++;
  }
  fclose(file);

  Position anti_nodes[ANTENNAS_INSTANCES*ANTENNAS_INSTANCES]; 
  int anti_nodes_count = 0;
  for (int i = 0; i < ANTENNAS_HM_SIZE; i++){
    Positions * p = &ANTENNAS_MAP[i];
    if (!p->active) continue;
    for (int j = 0; j < p->count; j++){
      Position * pos_j = &p->list[j];
      for (int k = 0; k < p->count; k++){
        if (j == k) continue;
        Position * pos_k = &p->list[k];
        int x1 = pos_j->x;
        int x2 = pos_k->x;
        int y1 = pos_j->y;
        int y2 = pos_k->y; 
        
        if (is_part2){
          int r = 1; 
          int ax1 = x1 + (r * (x2-x1));
          int ay1 = y1 + (r * (y2-y1));
          bool inbound_1 = in_bounds(ax1, w) && in_bounds(ay1, h);
          
          while (inbound_1){
            ax1 = x1 + (r * (x2-x1));
            ay1 = y1 + (r * (y2-y1));
            inbound_1 = in_bounds(ax1, w) && in_bounds(ay1, h);

            if (inbound_1) 
              anti_nodes[anti_nodes_count++] = (Position){.x=ax1, .y=ay1};

            r++; 
          }
          continue; 
        }
       
        int ax1 = x1 + (2 * (x2-x1));
        int ay1 = y1 + (2 * (y2-y1));
        int ax2 = x2 + (2 * (x1-x2));
        int ay2 = y2 + (2 * (y1-y2));
       
        if (in_bounds(ax1, w) && in_bounds(ay1, h)) 
          anti_nodes[anti_nodes_count++] = (Position){.x=ax1, .y=ay1};

        if (in_bounds(ax2, w) && in_bounds(ay2, h))
          anti_nodes[anti_nodes_count++] = (Position){.x=ax2, .y=ay2};

      }
    }
  }
  
  if (anti_nodes_count == 0) return 0; 

  //sort positions
  qsort(anti_nodes, anti_nodes_count, sizeof(Position), compare_positions);
  for (int i = 0; i < anti_nodes_count; i++){
    Position * p = &anti_nodes[i];
    printf("(%d,%d)\n", p->x, p->y);
  }

  printf("----count:%d\n", anti_nodes_count);
  int distinct = 1; int prev = 0;
  for (int i = 1; i < anti_nodes_count; i++){
    Position * p1 = &anti_nodes[prev++];
    Position * p2 = &anti_nodes[i];
    if (p1->x == p2->x && p1->y == p2->y){
      continue; 
    }
    distinct++;
  }
  return distinct;
}

void test_process_file(char * filename, char * tag, int expected, bool is_part2){
  int actual = process_file(filename, is_part2);
  printf("actual: %d, expected: %d\n", actual, expected);
  assert(actual == expected);
  printf("[TEST test_process_file]: TAG: %s - Passed\n", tag); 
}

int main(void){
  if (IS_TEST){
    test_process_file("day8_test1.txt", "TEST1", 14, false);
    test_process_file("day8_test2.txt", "TEST2", 2, false);
    test_process_file("day8_test1.txt", "TEST3", 34, true);
    return 0;
  }
  int result = process_file("day8_act.txt", false);
  int result2 = process_file("day8_act.txt", true); 
  printf("result: %d\n", result);
  printf("result 2 : %d\n", result2); 
  return 0; 
}
