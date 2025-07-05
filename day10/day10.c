#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define DO_TEST true
#define STACK_SIZE 500

//void get_neighbors(Grid * g, int * neighbors, int node);
  
enum Direction {
  NORTH = 0,
  EAST  = 1,
  WEST  = 2,
  SOUTH = 3
};
  
typedef struct {
  int * list;
  int size;
  int w;
  int h; 
} Grid;

int get_x(Grid * g, int i){
  return ((i) % (g->w));
}

int get_y(Grid * g, int i){
  return ((i) / (g->w)); 
}

void print_grid(Grid * grid, bool showCoordinates){
  printf("grid-size: %d\ngrid-w:%d\ngrid-h:%d\n\n", grid->size, grid->w, grid->h);
  for (int i = 0; i < grid->size; i++){
    if (showCoordinates) 
      printf("%d (%d,%d)\t", grid->list[i], get_x(grid, i), get_y(grid,i)); 
    else
      printf("%d", grid->list[i]);
    if (((i+1) % (grid->w)) == 0){
      printf("\n");
    }
  }
  printf("\n");
}

Grid * read_grid(char * filename, int mem){
  Grid * g = (Grid *) malloc(sizeof(Grid));
  g->list = (int *) malloc(sizeof(int)*mem);
  g->size = 0;
  g->w = 0;
  g-> h = 0;
  
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    printf("failed opening file: %s\n", filename);
    return g;
  }

  char line[1024];
  while (fgets(line, sizeof(line), file)){
    line[strcspn(line, "\n")] = '\0';
    g->w = strlen(line);
    for (int i = 0; i < g->w; i++){
      g->list[(g->size)++] = line[i]-'0'; 
    }
    (g->h)++; 
  }
  fclose(file); 
  return g; 
}

bool in_visited(int * visited, int n, int node, int height){
  if (height == 9) return false;
  for (int i = 0; i < n; i++){
    if (visited[i] == node) return true; 
  }
  return false;
}

void get_neighbors(Grid * g, int * neighbors, int node){
  int x = get_x(g, node);
  int y = get_y(g, node);

  for (int i = 0; i < 4; i++)
    neighbors[i] = -1;
  int w = g->w;
  int h = g->h; 
  if (y > 0)
    neighbors[NORTH] = node-w;
  if (y < (h-1))
    neighbors[SOUTH]  = node+w;
  if (x > 0)
    neighbors[WEST]  = node - 1; 
  if (x < (w-1))
    neighbors[EAST] = node + 1;
}

int traverse(Grid * g, int start, bool isPart2){
  int * frontier = (int *) malloc(sizeof(int) * g->size);
  int frontier_count = 0;
  
  int * visited = (int *) malloc(sizeof(int) * g->size);
  int visited_count = 0;
  frontier[frontier_count++] = start;
  
  int paths_found = 0;

  while (frontier_count > 0){
    int node = frontier[--frontier_count];
    frontier[frontier_count+1] = -1;
    if (node == -1) continue;
    visited[visited_count++] = node;
    int node_height = g->list[node];
    if (node_height == 9){
      paths_found++;
      continue; 
    }
    int neighbors[4];
    get_neighbors(g, neighbors, node);
    for (int i = 0; i < 4; i++){
      if (neighbors[i] == -1) continue;
      int neighbor = neighbors[i]; 
      int neighbor_height = g->list[neighbor];
      if (isPart2){
        if (((neighbor_height-node_height) == 1) && !in_visited(visited, visited_count, neighbor, neighbor_height)){
          frontier[frontier_count++] = neighbor;
        }
      }else if (((neighbor_height-node_height) == 1) && !in_visited(visited, visited_count, neighbor, -1)) {
        frontier[frontier_count++] = neighbor; 
      } 
    }
  }
  
  free(visited);
  free(frontier);
  
  return paths_found; 
}

void free_grid(Grid * g){
  free(g->list);
  free(g);
}

int find_score(Grid * g, bool isPart2){
  int sum = 0;
  for (int i = 0; i < g->size; i++){
    if (g->list[i] != 0) continue;
    sum += traverse(g, i, isPart2);
  }
  return sum;
}


void test_find_score(void){
  Grid * g = read_grid("day10_test.txt", 10000);
  print_grid(g, false);
  int score = find_score(g, false); 
  
  assert(score == 36);
  printf("[TEST test_find_score]: Passed\n");

  int score2 = find_score(g, true);
  printf("actual: %d, expected: %d\n", score2, 81);
  assert(score == 81);
  printf("[TEST test_find_score 2]: Passed\n");
  
  free_grid(g);
}

int main(void){
  if (DO_TEST){
    test_find_score();
    return 0; 
  }
  Grid * g = read_grid("day10_act.txt", 10000);
  print_grid(g, false);
  int score = find_score(g, false); 
  free_grid(g);
  printf("result:%d\n", score);
  return 0;
}
