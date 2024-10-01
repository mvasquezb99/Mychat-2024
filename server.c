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

#define PORT "5000"
#define BACKLOG 10 // Pending connections queue

void sigchld_handler(int s) {
  while (waitpid(-1, NULL, WNOHANG) > 0)
    ;
}

typedef struct {
  int server_listener;
  char *buf;
} thread_arg;

void *get_in_addr(struct sockaddr *their_addr) {
  if (their_addr->sa_family == AF_INET) {
    struct sockaddr_in *their_addr_in = (struct sockaddr_in *)
        their_addr; // We can cast a sockaddr structure to a sockaddr_in
                    // structure and viceversa.
    return &(their_addr_in->sin_addr);
  }

  struct sockaddr_in6 *their_addr_in6 = (struct sockaddr_in6 *)their_addr;
  return &(their_addr_in6->sin6_addr);
}
/*
 * This function recives a port number (5000 in this case) it also can be a
 * protocol like "http" or "https" and gets the desiganted Linked List from the
 * getaddrinfo() function, this Singly Linked List contains a set of results
 * which we have to iterate and test socket initialization.
 * */
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
int init_socket(struct addrinfo *serv_info) {
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

void *thread_listen(void *args) {
  printf("Inside listen!");

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

  return NULL;
}

void *func(void *vargp) {
  sleep(1);
  printf("Dentro del hilo");
  return NULL;
}

int main(int argc, char *argv[]) {
  struct addrinfo *serv_info; // getaddrinfo parameters and p as the poitner
                              // to iterate in the LL
  int server_socket,
      server_listener; // File decriptors for the connection and binding
  struct sockaddr_storage their_addr; // Connectors address infromation
  socklen_t sin_size;
  struct sigaction sa;
  int yes = 1;
  char buf[100];
  int numBytes;
  char ipstr[INET6_ADDRSTRLEN]; // Clients IP direction

  serv_info = get_server_info(PORT); // Get addrinfo struct
  server_socket =
      init_socket(serv_info); // Create, configure and Bind the socket

  // With the socket initialized we dont need the LL anymore
  freeaddrinfo(serv_info);

  if (listen(server_socket, BACKLOG) == -1) {
    perror("listen");
    exit(1);
  }

  printf("server: waiting for connections...\n");

  while (1) {
    sin_size = sizeof their_addr;
    server_listener = accept(server_socket, (struct sockaddr *)&their_addr,
                             &sin_size); // Este socket es para mandar y recibir

    if (server_listener == -1) { // Mientras que no tenga una conexiòn siempre
                                 // va a entrar a este If
      perror("accept");
      continue;
    }

    // Crear el hilo Y seguir con su vida

    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr),
              ipstr, sizeof ipstr);
    printf("server: got connection from %s\n", ipstr);
    close(server_socket);

    // Inicializar la estrucutra con los parametros para poder enviar a los
    // hilos
    thread_arg *args = malloc(sizeof *args);
    args->buf = buf;
    args->server_listener = server_listener;

    pthread_t thread_id1;
    pthread_create(&thread_id1, NULL, thread_listen, args);
    pthread_join(thread_id1, NULL);

    printf("server: Recived '%s'\n", args->buf);
    exit(1);
    return 0;
  }
}
