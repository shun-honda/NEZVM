// This file based on https://github.com/petewarden/c_hashmap

#ifndef HASHMAP_H
#define HASHMAP_H

#define MAP_MISSING -3  /* No such element */
#define MAP_FULL -2 	/* Hashmap is full */
#define MAP_OMEM -1 	/* Out of Memory */
#define MAP_OK 0 	/* OK */

typedef void *any_t;
typedef any_t map_t;

map_t new_hashmap();
void dispose_hashmap(map_t in);
int hashmap_length(map_t in);
int hashmap_put(map_t in, char* key, any_t value);
int hashmap_get(map_t in, char* key, any_t *arg);
int hashmap_remove(map_t in, char* key);
#endif