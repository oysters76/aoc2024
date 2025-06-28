#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

typedef long long LL;

typedef struct {
  int id;
  int start;
  int end;
  bool isFree;
  int adj_size;
} Block; 

int char_to_int(char c){
  return c - '0'; 
}

char int_to_char(int x){
  if (x < 0 || x > 9) return '0'; 
  return x + '0';
}

bool is_finish(int * blocks, int size){
  bool isEmptyFound = false;
  for (int i = 0; i < size; i++){
    if (blocks[i] == -1){
      isEmptyFound = true;
    }
    if (isEmptyFound && blocks[i] != -1) return false; 
  }
  return true;
}

LL process_file(char * filename, int mem){
  char * input_str = malloc(mem * sizeof(char)); 
  int input_count = 0; 
  
  FILE * file = fopen(filename, "r");
  if (file == NULL) { return -1; }
  char c;
  while (fread(&c, sizeof(char), 1, file) == 1){ 
    if (input_count >= mem) return -1;
    input_str[input_count++] = c;
  }
  
  fclose(file);
  
  int * blocks = malloc((mem*mem) * sizeof(int));
  int blocks_count = 0;
  int id = 0;
  for (int i = 0; i < input_count; i++){
    printf("-- %d\n", input_str[i]);
  }
  
  for (int i = 0; i < input_count; i++){
    int x = char_to_int(input_str[i]);
    if (i % 2 == 1){
      for (int k = 0; k < x; k++){
        blocks[blocks_count++] = -1;
      }
    }else{
      for (int k = 0; k < x; k++){
        blocks[blocks_count++] = id;
      }
      id++;
    }
  }

  int left = 0;
  int right = 0;

  for (int i = 0; i < blocks_count; i++){
    if (blocks[i] == -1){
      left = i;
      break; 
    }
  }
  for (int i = blocks_count-1; i >= 0; i--){
    if (blocks[i] != -1){
      right = i;
      break; 
    }
  }

  while (!is_finish(blocks, blocks_count)){

    blocks[left] = blocks[right];
    blocks[right] = -1;
    
    for (int i = 0; i < blocks_count; i++){
      if (blocks[i] == -1){
        left = i;
        break; 
      }
    }

    for (int i = blocks_count-1; i >= 0; i--){
      if (blocks[i] != -1){
        right = i;
        break; 
      }
    }
  }

  LL checksum = 0;
  for (int i = 0; i < blocks_count; i++){
    if (blocks[i] == -1) continue;
    checksum += (i * blocks[i]); 
  }
  free(input_str);
  free(blocks);
  return checksum;
}

int get_block_size(Block * b){
  int s = b->start;
  int e = b->end;
  return (e-s);
}

int compare_blocks_id_desc(const void *a, const void *b){
  const Block *b1 = *(const Block **)a;
  const Block *b2 = *(const Block **)b;
  return b2->id - b1->id;
}

void print_blocks(Block * blocks, int count){
  printf("\n");
  for (int i = 0; i < count; i++){
    Block b = blocks[i];
    int size = get_block_size(&b);
    for (int k = 0; k < size; k++){
      if (b.isFree){
        printf("."); 
      }else{
        if (b.adj_size != -1 && k >= b.adj_size){
          printf(".");
          continue;
        }
        printf("%d", b.id); 
      }
    }
  }
  printf("\n");
}

LL process_file2(char * filename, int mem){
  char * input_str = malloc(mem * sizeof(char)); 
  int input_count = 0; 
  
  FILE * file = fopen(filename, "r");
  if (file == NULL) { return -1; }
  char c;
  while (fread(&c, sizeof(char), 1, file) == 1){ 
    if (input_count >= mem) return -1;
    input_str[input_count++] = c;
  }
  
  fclose(file);

  Block * blocks = (Block *) malloc(mem * sizeof(Block));
  int block_count = 0; 

  int id = 0; 
  for (int i = 0; i < input_count; i++){
    int x = char_to_int(input_str[i]);
    if ((i%2) == 1){
      blocks[block_count++] = (Block) { .id=-1, .start=i, .end=(i+x), .isFree=true, .adj_size=-1 }; 
    }else{
      //block
      blocks[block_count++] = (Block) { .id = id, .start=i, .end=(i+x), .isFree=false, .adj_size=-1 };
      id++;
    }
  }
  
  int left = 0;

  for (int i = 0; i < block_count; i++){
    if (blocks[i].isFree){
      left = i;
      break; 
    }
  }

  Block ** block_list = (Block **) malloc(mem*sizeof(Block*));
  int block_list_count = 0; 
  for (int i = 0; i < block_count; i++){
    if (!blocks[i].isFree){
      block_list[block_list_count++] = &blocks[i]; 
    }
  }

  qsort(block_list, block_list_count, sizeof(Block *), compare_blocks_id_desc);
  
  int right = 0;
  print_blocks(blocks, block_count); 

  
  free(input_str);
  free(blocks);
  free(block_list);
  
  return 0; 
}

#define IS_TEST true

int main(void){
  if (IS_TEST){
    LL actual = process_file("day9_test.txt", 10000);
    printf("expected: %d, actual: %lld\n", 1928, actual); 
    assert(actual == 1928); 
    printf("PASSED test 1\n");

    LL actual2 = process_file2("day9_test.txt", 10000);
    printf("expected: %d, actual: %lld\n", 2858, actual2);
    assert(actual2 == 2858);
    printf("PASSED test 2\n");
    return 0;
  }
  printf("result: %lld\n", process_file("day9_act.txt", 120000));
  return 0; 
}
