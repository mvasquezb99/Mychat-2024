#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <threads.h>
#include <time.h>
#include <unistd.h>

struct addrinfo *get_server_info(char port[5]) {
  struct addrinfo hints;
  struct addrinfo *serv_info;
  int status;

  memset(&hints, 0, sizeof hints); // Make sure the struct is empty
  hints.ai_family = AF_UNSPEC;     // don't care abt IP versions
  hints.ai_socktype =
      SOCK_STREAM;             // Socktype, in this case STREAM SOCKET for TCP
  hints.ai_flags = AI_PASSIVE; // fill in my IP for me

  if ((status = getaddrinfo(NULL, port, &hints, &serv_info)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
  }
  return serv_info;
}

/* This method iterating thru the LL looking for a result that allows the
 * creation and binding of the socket All of the operations need to check for
 * errors.
 *
 * Binds a socket
 * */
int init_socket_server(struct addrinfo *serv_info) {
  struct addrinfo *p;
  int sockfd;
  int yes = 1;

  for (p = serv_info; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
      perror("setsockopt");
      exit(1);
    }

    if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("server: bind");
      continue;
    };
    break;
  }

  if (p == NULL) {
    fprintf(stderr, "server: failed to bind \n");
  }

  return sockfd;
}

int init_socket_client(struct addrinfo *serv_info) {
  struct addrinfo *p;
  int sockfd;
  int yes = 1;

  for (p = serv_info; p != NULL; p = p->ai_next) {
    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
      perror("server: socket");
      continue;
    }

    if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
      close(sockfd);
      perror("client: connect");
      continue;
    };
    break;
  }

  if (p == NULL) {
    fprintf(stderr, "client: failed to connect \n");
  }

  return sockfd;
}

typedef struct {
  int server_listener;
  char *buf;
} thread_arg;

void *thread_listen(void *args) {
  printf("Thread in control \n");

  thread_arg *actual_args = args;
  int server_listener = actual_args->server_listener;
  char burf[100];

  if ((send(server_listener, "SYNC \n", 13, 0)) == -1) {
    perror("send");
  }

  if ((recv(server_listener, &burf, 100 - 1, 0)) == -1) {
    perror("recv");
    exit(1);
  }

  actual_args->buf = burf; // Para retornar algun valor desde el hilo

  printf("%s", burf);

  return NULL;
}
