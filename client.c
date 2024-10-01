#include "myChatProtocol.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdbool.h>
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
int main(int argc, char *argv[]) {
  int client_socket, server_socket;
  struct addrinfo *serv_info;
  struct sockaddr_storage their_addr; // Connectors address infromation
  char user_name[50];

  serv_info = get_server_info(SERVERPORT);
  server_socket =
      init_socket_client(serv_info); // Crear el socket al servidor PUERTO 5000

  freeaddrinfo(serv_info);

  printf("Ingresa tu nombre de usuario: ");
  scanf("%s", user_name);
  printf("Bienvenido: %s \n", user_name);

  // Primera conexion. Deberia de devolverle la tabla de conexion y un mensaje
  // de que se conecto.
  sync_parameters *parameters = malloc(sizeof *parameters);
  parameters->name = user_name;
  parameters->method = "SYNC";
  parameters->disp = 0;
  parameters->server_socket = server_socket;
  sync_client(*parameters);

  return 0;
}
