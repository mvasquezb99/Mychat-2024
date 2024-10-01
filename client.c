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

#define SERVERPORT "5000"

int main(int argc, char *argv[]) {
  int client_socket, server_socket;
  struct addrinfo *serv_info;

  serv_info = get_server_info(SERVERPORT);
  server_socket =
      init_socket_client(serv_info); // Crear el socket al servidor PUERTO 5000

  return 0;
}
