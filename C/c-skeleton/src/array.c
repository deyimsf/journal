#include <array.h>


DArray *
DArray_create(size_t element_size, size_t initail_max)
{
	//为动态数组本身的结构分配内存
	DArray *array = malloc(sizeof(DArray));
	array->max = initial_max;	

	//分配一块内存,用来存储容器中的元素指针
	array->contents = calloc(initial_max, sizeof(void *));
	//当前有效元素的最后一个下标
	array->end = 0;
	array->element_size = element_size;
	array->expand_rate = DEFAULT_EXPAND_RATE;
	
	return array;
}


void
DArray_clear(DArray *array)
{
	int i = 0;
	if(array->element_size > 0){
		for(i = 0; i < array->max; i++){
			if(array->contents[i] != NULL){
				free(array->contents[i]);
			}	
		}
	}
}


static inline int 
DArray_resize(DArray *array, size_t newsize)
{
	array->max = newsize;
	
	void *contents = realloc(array->contents, array->max * sizeof(void *));
	
	array->contents = contents;
	
	return 0;
}


int 
DArray_expand(DArray *array)
{
	size_t old_max = array->max;
	DArray_resize(array, array->max + array->expand_rate)
	memset(array->contents + old_max, 0, array->expand_rate + 1);	

	return 0;
}


int
DArray_contract(DArray *array)
{
	int new_size = array->end < (int)array->expand_rate ? (int)array->expand_rate : array->end;

	return DArray_resize(array, new_size + 1);
}


void 
DArray_destroy(DArray *array)
{
	if(array) {
		if(array->contents){
			free(array->contents);
		}
		free(array);
	}
}


void 
DArray_clear_destroy(DArray *array)
{
	DArray_clear(array);
	DArray_destroy(array);
}


int
DArray_push(DArray *array, void *el)
{
	array->contents[array->end] = el;
	array->end++;

	if(DArray_end(array) >= DArray_max(array)) {
		return DArray_expand(array);
	} else {
		return 0;
	}
}


void *
DArray_pop(DArray *array)
{	
	void *el = DArray_remove(array, array->end - 1);	
	array->end--;

	if(DArray_end(array) > (int)array->expand_rate && DArray_end(array) % array->expand_rate){
		DArray_contract(array);
	}
	
	return el;
}




