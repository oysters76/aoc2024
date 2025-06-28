#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdbool.h>

#define DO_TEST   false
#define BUFF_SIZE 100000

typedef long long LL;

typedef struct {
  int id; 
  int start;
  int length; 
} File;

typedef struct {
  int count;
  File * list;
  int width;
} Files; 

int char_to_int(char c){
  return c - '0'; 
}

void print_files(Files * f){
  printf("\n");
  for (int i = 0; i < f->count; i++){
    for (int j = 0; j < f->list[i].length; j++){
      if (f->list[i].id == -1){
        printf("."); 
      }else{
        printf("%d", f->list[i].id);
      }
    }
  }
  printf("\t total-width:%d\n", f->width);
}


Files init_files(char * str, int size, int max_capacity){
  Files files = {0};
  files.list = (File *) malloc(sizeof(File) * max_capacity); 
  files.count = 0;
  int id = 0;
  int width = 0;
  int index = 0;
  for (int i = 0; i < size; i++){
    int l = char_to_int(str[i]); 
    if (i % 2 == 1){
      files.list[files.count++] = (File) {.id=-1, .start=index, .length=l};
      index += l;
    }else{
      width += l;
      files.list[files.count++] = (File) {.id=id, .start=index, .length=l};
      id++;
      index += l;
    }
  }
  files.width = width;
  return files;
}

int compare_start_positions(const void *a, const void *b){
  const File *f1 = (const File *)a;
  const File *f2 = (const File *)b;
  return f1->start - f2->start;
}

void move(Files * f, int id){
  int ind = -1;
  for (int i = 0; i < f->count; i++){
    if (f->list[i].id == id){
      ind = i;
      break; 
    }
  }
  if (ind == -1) return;
  File * file_to_move = &f->list[ind];
  int file_size = file_to_move->length; 
  for (int i = 0; i < f->count; i++){
    if (f->list[i].id != -1) continue;
    if (f->list[i].start > file_to_move->start) break;
    if (f->list[i].length >= file_size){
      int rem_space = f->list[i].length - file_size;
      f->list[i].id = file_to_move->id; 
      f->list[i].length = file_size;
      file_to_move->id = -1;

      if (rem_space > 0){
        printf("rem_space:%d\n", rem_space);
        printf("expr: %d->%d\n",f->list[i].start, f->list[i].start+(file_size));
        f->list[f->count++] = (File) { .id=-1, .start=(f->list[i].start+(file_size-1)), .length=rem_space};
        qsort(f->list, f->count, sizeof(File), compare_start_positions); 
      }
      break; 
    }
  }
}

int compare_ids_desc(const void *a, const void *b){
  return (*(const int *)b - *(const int *)a);
}

void compress(Files * files){
  int * ids = (int *) malloc(sizeof(int) * files->count);
  int ids_count = 0;

  for (int i = 0; i < files->count; i++){
    if (files->list[i].id == -1) continue;
    ids[ids_count++] = files->list[i].id; 
  }

  qsort(ids, ids_count, sizeof(int), compare_ids_desc);

  for (int i = 0; i < ids_count; i++){
    int id_to_move = ids[i];
    move(files, id_to_move);
  }
  
  //cleanup
  free(ids);
}

LL compute_checksum(Files * f){
  LL sum = 0;
  int index = 0;
  for (int i = 0; i < f->count; i++){
    if (f->list[i].id == -1){
      index += f->list[i].length;
      continue; 
    }
    for (int k = 0; k < f->list[i].length; k++){
      sum += (index * f->list[i].id);
      index++;
    }
  }
  return sum; 
}

void free_files(Files * f){
  free(f->list);
}

void run_test(void){
  char * testcase = "2333133121414131402";
  int testcase_size = strlen(testcase);
  Files files = init_files(testcase, testcase_size, 10000);
  compress(&files);
  LL checksum = compute_checksum(&files);
  free_files(&files);
  assert(checksum == 2858);
  printf("TEST PASS\n");
}

int main(void){
  if (DO_TEST){
    run_test();
    return 0; 
  }
  char * actual = (char *) malloc(sizeof(char) * BUFF_SIZE);
  FILE *fz = fopen("day9_in.txt", "r");
  if (fz == NULL){
    return -1; 
  }
  int ch;
  int a = 0;
  while ((ch = fgetc(fz)) != EOF){
    actual[a++] = ch;
  }
  printf("her");
  Files f = init_files(actual, a, a*a);
  printf("test");
  compress(&f);
  LL result = compute_checksum(&f);
  printf("result: %lld\n", result);
  
  return 0;
}
