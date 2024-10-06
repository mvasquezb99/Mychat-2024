#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TABLE_SIZE 10 // Tamaño de la tabla hash

typedef struct metaData {
  int disp;
  int socket;
} metaData;

typedef struct Node {
  char *key;
  metaData *value;
  struct Node *next;
} Node;

typedef struct HashTable {
  Node **table;
  pthread_mutex_t lock;
} HashTable;

unsigned int hash(const char *key) {
  unsigned long int hashval = 0;
  while (*key) {
    hashval = (hashval << 5) + *key++;
  }
  return hashval % TABLE_SIZE;
}

HashTable *create_table() {
  HashTable *ht = malloc(sizeof(HashTable));
  ht->table = calloc(TABLE_SIZE, sizeof(Node *));
  pthread_mutex_init(&ht->lock, NULL);
  return ht;
}

void insert(HashTable *ht, const char *key, int disp, int socket) {
  unsigned int index = hash(key);
  Node *new_node = malloc(sizeof(Node));
  new_node->key = strdup(key);

  metaData *new_metaData = malloc(sizeof *new_metaData);
  new_metaData->disp = disp;
  new_metaData->socket = socket;

  new_node->value = new_metaData;

  // Zona critica (Debe de haber un lock)
  pthread_mutex_lock(&ht->lock);
  new_node->next = ht->table[index];
  ht->table[index] = new_node;
  pthread_mutex_unlock(&ht->lock);
}

metaData *search(HashTable *ht, const char *key) {
  unsigned int index = hash(key);
  Node *current = ht->table[index];
  while (current) {
    if (strcmp(current->key, key) == 0) {
      return current->value;
    }
    current = current->next;
  }
  return NULL;
}

char *get_all_keys(HashTable *ht) {
  int estimated_size = TABLE_SIZE * 50;
  char *result = malloc(estimated_size);
  result[0] = '\0'; // Initialize the result string

  for (int i = 0; i < TABLE_SIZE; i++) {
    Node *current = ht->table[i];
    while (current) {
      strcat(result, current->key);
      if (current->next || i < TABLE_SIZE - 1) {
        strcat(result, "\n");
      }
      current = current->next;
    }
  }

  return result;
}

void free_table(HashTable *ht) {
  for (int i = 0; i < TABLE_SIZE; i++) {
    Node *current = ht->table[i];
    while (current) {
      Node *temp = current;
      current = current->next;
      free(temp->key);
      free(temp->value);
      free(temp);
    }
  }
  free(ht->table);
  free(ht);
}
