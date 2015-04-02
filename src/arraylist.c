// This file is based on json-c/arraylist.c [1]
// [1] https://github.com/json-c/json-c/blob/master/arraylist.c

#include "arraylist.h"
#include <stdlib.h>
#include <string.h>

array_list* new_array_list(dispose_array_list_func_t *f) {
	array_list* list = (array_list *)calloc(1, sizeof(array_list));
	if(!list) {
		return NULL;
	}
	list->size = ARRAY_LIST_DEFAULT_SIZE;
	list->length = 0;
	list->f = f;
	list->array = (void**)calloc(sizeof(void*), list->size);
	if(!list->array) {
		free(list);
		return NULL;
	}
	return list;
}

void dispose_array_list(array_list* list) {
	for(int i = 0; i < list->length; i++) {
		if(list->array[i]) {
			list->f(list->array[i]);
		}
	}
	free(list->array);
	free(list);
}

static int array_list_realloc(struct array_list *list, int max)
{
  void *array;
  int new_size;

  new_size = list->size << 1;
  if (new_size < max) {
    new_size = max;
  }
  array = realloc(list->array, new_size*sizeof(void*));
  if(!array) {
  	return -1;
  }
  list->array = (void**)array;
  (void)memset(list->array + list->size, 0, (new_size-list->size)*sizeof(void*));
  list->size = new_size;
  return 0;
}

int array_list_add_idx(array_list* list, void* e, int i) {
	int max = i+1;
	if(max < list->size) {
  	if(array_list_realloc(list, max)){
  		return -1;
  	}
  }
	if(list->array[i]) {
		list->f(list->array[i]);
	}
	list->array[i] = e;
	if(list->length < i) {
		list->length = i+1;
	}
	return 0;
}

int array_list_add(array_list* list, void* e) {
	return array_list_add_idx(list, e, list->length);
}

void* array_list_get(array_list* list, int i) {
	if(i >= list->length) {
		return NULL;
	}
	return list->array[i];
}

int array_list_length(array_list* list) {
	return list->length;
}
