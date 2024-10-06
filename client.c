#include "myChatProtocol.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
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

int main(int argc, char *argv[]) {
  int server_socket, i;
  struct addrinfo *serv_info;
  struct sockaddr_storage their_addr; // Connectors address infromation
  char user_name[MAX_USER_NAME], user_connect[MAX_USER_NAME];
  char burf[120], client_message[150];

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

  printf("\nCon quien quieres conectarte? Ingresa el nombre:");
  scanf("%s", user_connect);

  while (true) {
    printf("\nEscribir: ");
    scanf(" %[^\n]", client_message);

    con_client(user_connect, server_socket, client_message);

    if ((recv(server_socket, &burf, 120 - 1, 0)) == -1) {
      perror("recv");
      exit(1);
    }

    printf("mensaje recibido...\n");
    printf("%s", burf);
    burf[0] = '\0';
  }

  return 0;
}
