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
  new_node->next = ht->table[index];
  ht->table[index] = new_node;
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

/*
int main() {
HashTable *ht = create_table();
insert(ht, "Miguel", 1, 3);
insert(ht, "Pinenda", 0, 2);

  metaData *m1 = search(ht, "Miguel");
  if (m1) {
    printf("Disponibilidad: %d, #Socket: %d\n", m1->disp, m1->socket);
  }

  metaData *m2 = search(ht, "Pinenda");
  if (m2) {
    printf("Disponibilidad: %d, #Socket: %d\n", m2->disp, m2->socket);
  }

  free_table(ht);
  return 0;
}
*/
