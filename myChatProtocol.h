#ifndef myChatProtocol
#define myChatProtocol
#include "hashtable.h"
#include <stdbool.h>
struct addrinfo *get_server_info(char port[5]);
struct addrinfo *get_client_info(char port[5]);
int init_socket_server(struct addrinfo *serv_info);
int init_socket_client(struct addrinfo *serv_info);
void *thread_listen(void *args);
typedef struct {
  int server_listener;
  char *buf;
  struct HashTable *ht;
} thread_arg;
typedef struct {
  char *name;
  int disp;
  int socket;
} mssg_desencp;
void *sync_client(char user_name[50], int server_socket);
void *con_client(char user_connect[50], int server_socket,
                 char client_message[150]);
void *dcon_client(char user[50], int server_socket);
void trim(char *str);
#endif
