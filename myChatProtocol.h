#ifndef myChatProtocol
#define myChatProtocol
#include "hashtable.h"
#include <stdbool.h>
#define MAX_USER_NAME 10

struct addrinfo *get_server_info(char port[5]);
struct addrinfo *get_client_info(char port[5]);
int init_socket_server(struct addrinfo *serv_info);
int init_socket_client(struct addrinfo *serv_info);
void *thread_listen(void *args);
void *sync_client(char user_name[MAX_USER_NAME], int server_socket);
void *con_client(char user_connect[MAX_USER_NAME], int server_socket,
                 char client_message[150]);
void *dcon_client(char user[MAX_USER_NAME], int server_socket, char user_connect[MAX_USER_NAME],
                  int flag);
void trim(char *str);
void send_client(char server_message[150], int client_socket, int type);

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
typedef struct {
  char *name;
  int socket;
  char *user_connect;
} messg_dcon;

#endif
