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

#define SERVERPORT "5000"

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

int init_socket(struct addrinfo *serv_info) {
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

int main(int argc, char *argv[]) {
  int client_socket, server_socket;
  struct addrinfo *serv_info;

  serv_info = get_server_info(SERVERPORT);
  server_socket = init_socket(serv_info); // Crear el socket al servidor

  return 0;
}
