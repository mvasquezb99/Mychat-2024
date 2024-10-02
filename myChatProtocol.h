#ifndef myChatProtocol
#define myChatProtocol
#include "hashtable.h"
#include <stdbool.h>
struct addrinfo *get_server_info(char port[5]);
int init_socket_server(struct addrinfo *serv_info);
int init_socket_client(struct addrinfo *serv_info);
void *thread_listen(void *args);
typedef struct {
  int server_listener;
  char *buf;
  struct HashTable *ht;
} thread_arg;
typedef struct {
  char *method;
  char *name;
  int disp;
  int server_socket;
} sync_parameters;

typedef struct {
  char name[20];
  int disp;
  int socket;
} mssg_desencp;
void *sync_client(sync_parameters parameters);
#endif
