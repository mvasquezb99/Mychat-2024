#include "hashtable.h"
#include "myChatProtocol.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
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
HashTable *ht;
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

int main(int argc, char *argv[]) {
  struct addrinfo *serv_info; // getaddrinfo parameters and p as the poitner to
                              // iterate in the LL
  struct sockaddr_storage their_addr; // Connectors address infromation
  int server_socket,
      server_listener; // File decriptors for the connection and binding
  socklen_t sin_size;
  char ipstr[INET6_ADDRSTRLEN]; // Clients IP direction
  ht = create_table();          // Create the hashtable

  serv_info = get_server_info(PORT); // Get addrinfo struct
  server_socket =
      init_socket_server(serv_info); // Create, configure and Bind the socket

  freeaddrinfo(serv_info); // Liberar el espacio en memoria de serv_info

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
      perror("accept");
      continue;
    }

    inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr),
              ipstr, sizeof ipstr);

    printf("server: got connection from %s\n", ipstr);

    thread_arg *args = malloc(sizeof *args);
    args->server_listener = server_listener;
    args->ht = ht;

    pthread_t thread_id1;
    pthread_create(&thread_id1, NULL, thread_listen, args);
  }
}
