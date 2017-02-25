#undef NDEBUG
#include <stdint.h>


static int
default_compare(void *a, void *b)
{
	return bstrcmp((bstring)a, (bstring)b);
}


static uint32_t default_hash(void *a)
{
	size_t len = blength((bstring)a);

}


Hashmap *
Hashmap_create(Hashmap_compare compare, Hashmap_hash hash)
{
	Hashmap *map = calloc(1, sizeof(Hashmap));
	
	map->compare = comare==NULL?default_compare:compare;
	map->hash = hash==NULL?default_hash:hash;
	map->buckets = DArray_create(sizeof(DArray *),DEFAULT_NUMBER_OF_BUCKETS);
	map->buckets->end = map->buckets->max;
	
	return map;
}


void 
Hashmap_destroy(Hashmap *map)
{
	int i = 0;
	int j = 0;

	if(!map){
		return;
	}

	if(!map->buckets){
		return;
	}

	for(i = 0; i < DArray_count(map->buckets); i++){
		//用DArray作桶,但桶里的每个元素也是DArray
		//用DArray代替链表避免hash碰撞
		//最后一层的DArray里放的才是HashmapNode
		DArray *bucket = DArray_get(map->buckets, i);
		if(!bucket){
			continue;	
		}

		for(j = 0; j < DArray_count(bucket); j++){
			free(DArray_get(bucket,j));
		}
		DArray_destroy(bucket);
	}
}


static inline HashmapNode *Hashmap_node_create(int hash, void *key, void *data)
{
	HashmapNode *node = calloc(1, sizeof(HashmapNode));
	
	node->key = key;
	node->data = data;
	node->hash = hash;	
	
	return node;
}





