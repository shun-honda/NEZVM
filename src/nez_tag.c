#include "nezvm.h"

nez_tag* nez_new_tag() {
	nez_tag* tag = (nez_tag*)malloc(sizeof(nez_tag));
	tag->list = new_array_list(&nez_dispose_tag_entry);
	tag->map = new_hashmap();
	return tag;
}

void nez_dispose_tag(nez_tag* tag) {
	dispose_array_list(tag->list);
	dispose_hashmap(tag->map);
	free(tag);
}

nez_tag_entry* tag(nez_tag* tag, char* name) {
	nez_tag_entry* entry;
	if(hashmap_get(tag->map, name, (void**)&entry) == -3) {
		entry = nez_new_tag_entry(tag, name);
		hashmap_put(tag->map, name, (void**)&entry);
		array_list_add(tag->list, entry);
	}
	return entry;
}

nez_tag_entry* tag_id(nez_tag* tag, int id) {
	return array_list_get(tag->list, id);
}

nez_tag_entry* nez_new_tag_entry(nez_tag* tag, char* name) {
	nez_tag_entry* entry = (nez_tag_entry*)malloc(sizeof(nez_tag_entry));
	entry->tag_name = name;
	entry->id = hashmap_length(tag->map);
	return entry;
}

void nez_dispose_tag_entry(void* tag_entry) {
	free(tag_entry);
}