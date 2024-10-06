#include "hashtable.h"
#include <arpa/inet.h>
#include <ctype.h>
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

/*
This method invokes the getaddr info system call i order to get
all the neccesary information to later initialize a socket

initialize LL
returns addrinfo
*/
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

struct addrinfo *get_client_info(char port[5]) {
  struct addrinfo hints;
  struct addrinfo *serv_info;
  int status;

  memset(&hints, 0, sizeof hints); // Make sure the struct is empty
  hints.ai_family = AF_UNSPEC;     // don't care abt IP versions
  hints.ai_socktype =
      SOCK_STREAM;             // Socktype, in this case STREAM SOCKET for TCP
  hints.ai_flags = AI_PASSIVE; // fill in my IP for me

  // "172.20.10.3"
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
 * Return Socket
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

/* This method iterating thru the LL looking for a result that allows the
 * creation and connection of the socket All of the operations need to check for
 * errors.
 *
 * Connects to a socket
 * Return Socket
 * */
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
/*
Struct for returning from the thread to the parent
*/
typedef struct {
  int server_listener;
  char *buf;
  struct HashTable *ht;
} thread_arg;

/*
Struct to store the decapsulated message
*/
typedef struct {
  char *name;
  int disp;
  int socket;
} mssg_desencp;

void trim(char *str) {
  int inicio = 0;
  int fin = strlen(str) - 1;

  // Eliminar espacios al inicio
  while (isspace(str[inicio])) {
    inicio++;
  }

  // Eliminar espacios al final
  while (fin >= inicio && isspace(str[fin])) {
    fin--;
  }

  // Ajustar la cadena
  int length = fin - inicio + 1;
  memmove(str, str + inicio, length);
  str[length] = '\0'; // Terminar la cadena
}
/*
Main thread function! This function will analize the method space in the
protocol PDU and decapsulate it following the structure we decalare.
*/
void *thread_listen(void *args) {
  thread_arg *actual_args = args;
  int server_listener = actual_args->server_listener;
  HashTable *ht = actual_args->ht; // Remember to modify ht to save the changes
  char burf[120];
  char *method;
  char *message_info;

  while (true) {
    method = "\0";
    if ((recv(server_listener, &burf, 120 - 1, 0)) == -1) {
      perror("recv");
      pthread_exit(NULL);
    }

    actual_args->buf = burf;

    method = strtok(burf, ":"); // SYNC
    message_info = burf + 5;

    if (strcmp(method, "SYNC") == 0) {

      // printf("esta en thread, llego:%s", message_info);

      mssg_desencp myData;
      myData.name[0] = '\0';
      int count = 0;
      char temp[50];
      int length = 0;
      char copy[100];
      strcpy(copy, message_info);
      for (int i = 0; message_info[i] != '\0'; i++) {
        if (message_info[i] == ':') {
          count += 1;
          if (count == 1) {
            trim(temp);
            // printf("va a meter en el name: %s", temp);

            strcat(myData.name, temp);
            // printf("esta en el ciclo separando, llego username:%s",
            // myData.name);
            temp[0] = '\0';
          } else if (count == 2) {
            myData.disp = atoi(temp);
            temp[0] = '\0';
          }
        } else {
          length = 0;
          while (temp[length] != '\0') {
            length++;
          }
          temp[length] = message_info[i];
          temp[length + 1] = '\0';
        }
      }
      myData.socket = server_listener;

      // printf("va a meter a la hash:%s", myData.name);

      insert(ht, myData.name, myData.disp, myData.socket);
      metaData *entry = search(ht, myData.name);
      printf("\nName:%s, { Disponibilidad:%d, #Socket:%d }\n", myData.name,
             entry->disp, entry->socket);
      char *keys = get_all_keys(ht);
      printf("Clientes registrados:\n%s", keys);

      if ((send(myData.socket, keys, 100, 0)) == -1) {
        perror("send");
      }

      *burf = '\0';
    } else if (strcmp(method, "CONN") == 0) {
      char *user;
      char *message_cpy = (char *)malloc(strlen(message_info) + 1);
      char *user_message = (char *)malloc(120);
      strcpy(message_cpy, message_info);
      user = strtok(message_info, ":");
      // printf("%s\n", user);
      // printf("%s", message_cpy);
      metaData *entry = search(ht, user);
      if (entry == NULL) {
        perror("entry : User not found");
        pthread_exit(NULL);
      }
      printf("\nUser found => Name: %s, #Socket: %d \n", user, entry->socket);
      char *temp = malloc(strlen(message_cpy) + 1); // +1 for null-terminator
      if (temp == NULL) {
        printf("Memory allocation failed!\n");
        return 0;
      }

      strcpy(temp, message_cpy);

      char *token = strtok(temp, ":");
      int token_count = 0;
      char saved_message[100] = "";

      while (token != NULL) {
        token_count++;
        if (token_count == 2) {
          strncpy(saved_message, token, sizeof(saved_message) - 1);
          saved_message[sizeof(saved_message) - 1] = '\0';
        }
        token = strtok(NULL, ":");
      }
      free(temp);

      printf("Socket a enviar:%d, Mensaje: %s", entry->socket, saved_message);
      if ((send(entry->socket, saved_message, 100, 0)) == -1) {
        perror("send");
      }
    }
    else if (strcmp(method, "DCON") == 0) {
      printf("entro a dcon!!!!!!!!!!!!!");
    }
  }
  return NULL;
}

/*
Struct para poder enviar los parametros de encapsulamiento desde el cliente a el
protocolo.
*/

void *sync_client(char user_name[50], int server_socket) {
  char message[100];
  char socket[2];
  sprintf(socket, "%d", server_socket);

  strcpy(message, "SYNC");
  strcat(message, ":");
  strcat(message, user_name);
  strcat(message, ":");
  strcat(message, "0");
  strcat(message, ":");
  strcat(message, socket);
  strcat(message, ":");
  strcat(message, "END");

  // Envia el mensaje de SYNC al servidor
  // printf("esta en sync, enviando username:%s\n", message);
  if ((send(server_socket, message, 100, 0)) == -1) {
    perror("send");
  }

  return NULL;
}

void *con_client(char user_connect[50], int server_socket,
                 char client_message[150]) {
  char message[100];
  char socket[2];

  strcpy(message, "CONN");
  strcat(message, ":");
  strcat(message, user_connect);
  strcat(message, ":");
  strcat(message, client_message);
  strcat(message, ":");
  strcat(message, "END");

  // Envia el mensaje de SYNC al servidor
  if ((send(server_socket, message, 100, 0)) == -1) {
    perror("send");
  }
  return NULL;
}
// struct to save the decripted values of the message

void *dcon_client(char user[50], int server_socket) {
  char message[100];
  char socket[2];

  sprintf(socket, "%d", server_socket);

  strcpy(message, "DCON");
  strcat(message, ":");
  strcat(message, user);
  strcat(message, ":");
  strcat(message, socket);
  strcat(message, ":");
  strcat(message, "END");


  // Envia el mensaje de SYNC al servidor
  if ((send(server_socket, message, 100, 0)) == -1) {
    perror("send");
  }
  return NULL;
}
