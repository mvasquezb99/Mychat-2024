#include "hashtable.h"
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

  if ((recv(server_listener, &burf, 120 - 1, 0)) == -1) {
    perror("recv");
    pthread_exit(NULL);
  }

  actual_args->buf = burf;

  method = strtok(burf, ":"); // SYNC
  message_info = burf + 5;

  if (strcmp(method, "SYNC") == 0) {

    mssg_desencp myData;
    char copy[100];
    strcpy(copy, message_info);
    int count = 0;
    char temp[50];
    int length = 0;

    for (int i = 0; message_info[i] != '\0'; i++) {
      if (message_info[i] == ':') {
        count += 1;
        if (count == 1) {
          strcat(myData.name, temp);
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

    insert(ht, myData.name, myData.disp, myData.socket);
    // printf("\nNAME: %s, DISP: %d, SOCKET: %d\n", myData.name, myData.disp,
    //        myData.socket);
    metaData *entry = search(ht, myData.name);
    printf("\nName: %s, { Disponibilidad: %d, #Socket: %d }\n", myData.name,
           entry->disp, entry->socket);

    if ((send(myData.socket, "***You are connected!***\n", 100, 0)) == -1) {
      perror("send");
    }
  }
  return NULL;
}

/*
Struct para poder enviar los parametros de encapsulamiento desde el cliente a el
protocolo.
*/
typedef struct {
  char *method;
  char *name;
  int disp;
  int server_socket;
} sync_parameters;

void *sync_client(char user_name[50], int server_socket) {
  sync_parameters *parameters = malloc(sizeof *parameters);
  parameters->name = user_name;
  parameters->method = "SYNC";
  parameters->disp = 0;
  // Socket para cliente enviar a servidor
  parameters->server_socket = server_socket;

  char message[100];
  char socket[2];
  sprintf(socket, "%d", parameters->server_socket);

  strcpy(message, parameters->method);
  strcat(message, ":");
  strcat(message, parameters->name);
  strcat(message, ":");
  strcat(message, parameters->disp ? "1" : "0");
  strcat(message, ":");
  strcat(message, socket);
  strcat(message, ":");
  strcat(message, "END");

  // Envia el mensaje de SYNC al servidor
  if ((send(parameters->server_socket, message, 100, 0)) == -1) {
    perror("send");
  }

  return NULL;
}
// struct to save the decripted values of the message
