#ifndef myChatProtocol
#define myChatProtocol
#include <stdbool.h>
struct addrinfo *get_server_info(char port[5]);
int init_socket_server(struct addrinfo *serv_info);
int init_socket_client(struct addrinfo *serv_info);
void *thread_listen(void *args);
typedef struct {
  int server_listener;
  char *buf;
} thread_arg;
typedef struct {
  char *method;
  char *name;
  bool disp;
  int server_socket;
} sync_parameters;
void *sync_client(sync_parameters parameters);

#endif
