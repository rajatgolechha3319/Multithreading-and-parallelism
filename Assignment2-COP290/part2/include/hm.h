#include "mythread.h"
#include "list.h"
#define SZ 4096

struct hashmap_element_s {
  char *key;
  void *data;
};

struct hashmap_s {
  struct list* table[SZ];
  struct lock* lk[SZ];
};

// int compare_str(char* key1,char* key2){
//   int i = 0;
//   while(*(key1+i) != '\0' && *(key2+i) != '\0'){
//     if(*(key1+i) != *(key2+i)){
//       return 0;
//     }
//     i++;
//   }
//   if(*(key1+i) == '\0' && *(key2+i) == '\0'){
//     return 1;
//   }
//   return 0;
// }

int hash(const char* key){
  int value = 0;
  int i=0;
  while(*(key+i) != '\0'){
    value *= 29;
    value += *(key+i);
    i++;
  }
  return (value%4096);
}

int hashmap_create(struct hashmap_s *const out_hashmap){   // Initialize a hashmap
  for(int i = 0; i < SZ; i++) {
    //out_hashmap->table[i] = (struct list*) malloc(sizeof(struct list));
    out_hashmap->table[i] = list_new(); 
    //out_hashmap->lk[i] = (struct lock*) malloc(sizeof(struct lock));
    //out_hashmap->lk[i] = lock_new();
  }
  return 0;
}
int hashmap_put(struct hashmap_s *const hashmap, const char* key, void* data){   // Set value of the key as data in hashmap. You can use any method to resolve conflicts. Also write your own hashing function
  int hash_value = hash(key);
  struct hashmap_element_s *new_element = (struct hashmap_element_s*) malloc(sizeof(struct hashmap_element_s));
  int i = 0;
  while(*(key+i) != '\0'){
    i++;
  }
  char *key_value = (char*) malloc(sizeof(char)*(i+1));
  for(int j = 0; j <= i; j++){
    *(key_value+j) = *(key+j);
  }
  new_element->key = key_value;
  new_element->data = data;
  //struct listentry *new_entry = (struct listentry*) malloc(sizeof(struct listentry));
  //new_entry->data = new_element;
  struct listentry *faltu = list_add(hashmap->table[hash_value],new_element);
  return 0;
}
void* hashmap_get(struct hashmap_s *const hashmap, const char* key){    // Fetch value of a key from hashmap
  int hash_value = hash(key);
  struct listentry *entry = hashmap->table[hash_value]->head;
  while(entry != NULL){
    struct hashmap_element_s *element = (struct hashmap_element_s*) entry->data;
    int i=0;
    int flag = 0;
    char *key1 = element->key;
    while(*(key+i) != '\0' && *(key1+i) != '\0'){
      if(*(key+i) != *(key1+i)){
        flag = 1;
        break;
      }
      i++;
    }
    if(*(key+i) == '\0' && *(key1+i) == '\0' && flag == 0){
      return element->data;
    }
    entry = entry->next;
  }
  return NULL;
}
void hashmap_iterator(struct hashmap_s* const hashmap, 
                        int (*f)(struct hashmap_element_s *const))  // Execute argument function on each key-value pair in hashmap
{
  for(int i = 0; i < SZ; i++){
    struct listentry *entry = hashmap->table[i]->head;
    while(entry != NULL){
      struct hashmap_element_s *element = (struct hashmap_element_s*) entry->data;
      f(element);
    //  printf(i);
      entry = entry->next;
    }
  }
}
//int acquire_bucket(struct hashmap_s *const hashmap, const char* key);   // Acquire lock on a hashmap slot
//int release_bucket(struct hashmap_s *const hashmap, const char* key);   // Release acquired lock
