#include <stdio.h>
#include <glib.h>

int main(void){
  GArray *array = g_array_new(FALSE, FALSE, sizeof(int));

  for (int i = 0; i < 10; i++){
    g_array_append_val(array, i); 
  }

  for (int i = 0; i < array->len; i++){
    printf("%d->%d\n",i,g_array_index(array, int, i)); 
  }

  g_array_free(array, TRUE);
  return 0; 
}
