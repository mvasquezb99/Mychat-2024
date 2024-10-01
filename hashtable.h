#ifndef hashtable
#define hashtable

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10 // Tamaño de la tabla hash

typedef struct metaData {
  int disp;   // 1 para disponible, 0 para no disponible
  int socket; // Identificador del socket
} metaData;

typedef struct Node {
  char *key;         // Clave para identificar los MetaDatos
  metaData *value;   // Puntero a la estructura MetaDatos
  struct Node *next; // Puntero al siguiente nodo
} Node;

typedef struct HashTable {
  Node **table;
} HashTable;

unsigned int hash(const char *key);

HashTable *create_table();

void insert(HashTable *ht, const char *key, int disp, int socket);

metaData *search(HashTable *ht, const char *key);

void free_table(HashTable *ht);

#endif
