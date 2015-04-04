#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#define ARRAY_LIST_DEFAULT_SIZE 256

typedef void (dispose_array_list_func_t) (void *e);

struct array_list {
	void **array;
	int length;
	int size;
	dispose_array_list_func_t *f;
};

typedef struct array_list array_list;

array_list* new_array_list(dispose_array_list_func_t *f);
void dispose_array_list(array_list* list);
int array_list_add_idx(array_list* list, void* e, int i);
int array_list_add(array_list* list, void* e);
void* array_list_get(array_list* list, int i);
int array_list_length(array_list* list);

#endif