#include "myChatProtocol.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <threads.h>
#include <unistd.h>

#define SERVERPORT "5000"
#define MAX_USER_NAME 50

void *thread_recv(void *args) {
  thread_arg *actual_rcv = args;
  char local_buff[120];
  int local_server_listener = actual_rcv->server_listener;

  while (true) {
    if ((recv(local_server_listener, &local_buff, 120 - 1, 0)) == -1) {
      perror("recv");
      exit(1);
    }
    // printf("%s", local_buff);
    printf("-> %s \n", local_buff);
    local_buff[0] = '\0';
  }
  return NULL;
}
int main(int argc, char *argv[]) {
  int server_socket, i;
  struct addrinfo *serv_info;
  struct sockaddr_storage their_addr; // Connectors address infromation
  char user_name[MAX_USER_NAME], user_connect[MAX_USER_NAME];
  char burf[120], client_message[150];
  pthread_t listen_thread;
  thread_arg *rcv_args;

  serv_info = get_client_info(SERVERPORT);
  server_socket =
      init_socket_client(serv_info); // Crear el socket al servidor PUERTO 5000

  freeaddrinfo(serv_info);

  printf("Ingresa tu nombre de usuario:");
  scanf("%s", user_name);
  printf("\nBienvenido: %s\n", user_name);

  // Primera conexion. Deberia de devolverle la tabla de conexion y un mensaje
  // de que se conecto.
  sync_client(user_name, server_socket);

  if ((recv(server_socket, &burf, 120 - 1, 0)) == -1) {
    perror("recv");
    exit(1);
  }

  printf("Clientes disponibles: \n%s", burf);
  *burf = '\0';

  rcv_args = malloc(sizeof *rcv_args);
  rcv_args->buf = burf;
  rcv_args->server_listener = server_socket;

  pthread_t thread;
  pthread_create(&thread, NULL, thread_recv, rcv_args);

  printf("\nCon quien quieres conectarte? Ingresa el nombre:");
  scanf("%s", user_connect);

  while (true) {
    printf("\r");
    scanf(" %[^\n]", client_message);
    if (strcmp(client_message, "exit_") == 0) {
      break;
    } else {
      con_client(user_connect, server_socket, client_message);
    }
  }
  dcon_client(user_name, server_socket, user_connect);
  return 0;
}
