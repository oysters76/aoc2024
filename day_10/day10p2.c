#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define STACK_SIZE 1500

typedef struct {
  int * list;
  int size;
  int w;
  int h; 
} Grid;

enum Direction {
  NORTH = 0,
  EAST  = 1,
  WEST  = 2,
  SOUTH = 3
};

int get_x(Grid * g, int i){
  return ((i) % (g->w));
}

int get_y(Grid * g, int i){
  return ((i) / (g->w)); 
}

typedef struct Node {
  struct Node * parent;
  int value;
} Node; 

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

bool in_visited(int * visited, int n, int node){
  for (int i = 0; i < n; i++){
    if (visited[i] == node) return true; 
  }
  return false;
}

Node * init_node(Node * parent, int value){
  Node * node = (Node *) malloc(sizeof(Node));
  node->parent = parent;
  node->value = value;
  return node; 
}

int traverse(Grid * g, int index){
  int * visited = (int *) malloc(sizeof(int) * STACK_SIZE);
  int visited_count = 0; 
  Node ** stack = (Node **) malloc(sizeof(Node*) * STACK_SIZE);  
  int stack_ptr = 0;

  stack[stack_ptr++] = init_node(NULL, index);  //root  
  int nodes_size = 0;
  nodes_size++;

  
  int sum = 0;
  
  while (stack_ptr > 0){
    Node * cnode = stack[--stack_ptr]; 
    int node = cnode->value;
    int node_height = g->list[node]; 
    //printf("c: %d %d\n", node, node_height); 
    if (node_height == 9){
      //save the path
      Node * tnode = cnode;
      printf("\n");
      int lim = 0;
      while (tnode != NULL){
        printf(" (%d)%d -> ", g->list[tnode->value], tnode->value);
        tnode = tnode->parent;
        lim++;
        if (lim >= 15) break; 
      }
      printf("\n");
      sum++;
      continue; 
    }
    int neighbors[4];
    get_neighbors(g, neighbors, node);
    for (int i = 0; i < 4; i++){
      if (neighbors[i] == -1) continue;
      int neighbor = neighbors[i]; 
      int neighbor_height = g->list[neighbor];
      if (((neighbor_height-node_height) == 1)) {
        stack[stack_ptr++] = init_node(cnode, neighbor); 
        nodes_size++; 
      } 
    }
  }
  for (int k = 0; k < nodes_size; k++){
    free(stack[k]); 
  }
  free(visited);
  printf("sum: %d\n", sum);
  return sum; 
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

void free_grid(Grid * g){
  free(g->list);
  free(g);
}

int main(void){
 Grid * g = read_grid("day10_act.txt", 10000);
 int s = 0; 
 for (int i = 0; i < g->size; i++){
   if (g->list[i] != 0) continue;
   s += traverse(g, i); 
   printf("\n\n");
 }
 printf("S = %d\n", s);
 free_grid(g);
 return 0; 
}
