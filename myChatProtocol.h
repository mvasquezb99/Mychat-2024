#ifndef myChatProtocol
#define myChatProtocol

struct addrinfo *get_server_info(char port[5]);
int init_socket_server(struct addrinfo *serv_info);
int init_socket_client(struct addrinfo *serv_info);
void *thread_listen(void *args);
typedef struct {
  int server_listener;
  char *buf;
} thread_arg;
#endif
