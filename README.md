# Mychat-2024
## Introducción

En el contexto actual de la tecnología de la información, la comunicación en tiempo real entre usuarios es esencial. Este proyecto desarrolló una aplicación de chat utilizando el modelo cliente/servidor, donde se implementó un protocolo de capa de aplicación que facilitó la interacción entre múltiples clientes a través de un servidor central. La aplicación de chat es capaz de gestionar la comunicación en tiempo real, permitiendo que los usuarios envíen y reciban mensajes de manera eficiente y efectiva. A través de este desarrollo, se abordó el diseño y la implementación de aplicaciones concurrentes en red. Para ello, se empleó la API Sockets de Berkley, lo que permitió establecer un protocolo de comunicaciones que facilitó la interacción entre la aplicación cliente y la aplicación servidor.

## Desarrollo

### Contexto y decisión de implementación

Antes de entrar en detalles, debemos aclarar el porque se implementaron "STREAM Sockets" antes que "DGRAM Sockets", el debate se presenta ya que son los protocolos de capa de transporte que gestionará la transmisión de datos entre la aplicación. Por un lado TCP es un protocolo orientado a la conexión, que establece una conexión estable entre dos puntos antes de transmitir datos. Proporciona entrega garantizada, orden de los datos y corrección de errores. Esto lo hace adecuado para aplicaciones donde la fiabilidad es crítica. En contraparte UDP no es orientado a la conexión, implemente envía datagramas sin asegurarse de que lleguen a su destino o en el orden correcto. Es más rápido pero no garantiza la entrega de los datos, siendo útil en aplicaciones donde la pérdida ocasional de paquetes es aceptable (Ejemplos claros los video juegos o plataformas de video en vivo).

Ahora si partimos de que cada tipo respeta las funcionalidades mencionadas anteriormente de acuerdo a cada protocolo, decidimos que el uso de Stream Socket (TCP) es más apropiado porque:

1. *Orientación a la Conexión*: Los Stream Sockets (TCP) aseguran que exista una conexión estable antes de enviar o recibir cualquier mensaje. Esto garantiza que ambos clientes estén conectados de manera segura, lo cual es crucial en un chat donde los usuarios deben saber que sus mensajes serán entregados.

2. *Entrega Garantizada*: En un chat, es vital que cada mensaje enviado por un usuario llegue a su destinatario sin pérdida. TCP se encarga de retransmitir cualquier mensaje perdido, mientras que con UDP, los mensajes podrían perderse sin que el usuario se dé cuenta, lo que afectaría negativamente la experiencia de la conversación.

3. *Integridad y Orden de los Mensajes*: Con TCP, los mensajes se entregan en el mismo orden en el que fueron enviados, lo cual es esencial para mantener la coherencia de una conversación. UDP, por su naturaleza, no garantiza el orden, lo que podría desorganizar los mensajes y hacer confusa la comunicación entre los usuarios.

4. *Congestión de Errores y Control*: TCP tiene mecanismos para la detección de errores y ajuste dinámico de la tasa de envío en función de la congestión de la red. Esto permite que los mensajes sean transmitidos de manera confiable, incluso en redes con fluctuaciones. Esto es crucial para asegurar que la experiencia de chat sea fluida y sin interrupciones, algo que no se podría garantizar con UDP.

5. *Evitar Complejidad Adicional en la Implementación*: El uso de Stream Sockets (TCP) simplifica el desarrollo de la aplicación. Al utilizar UDP, tendríamos que implementar manualmente funciones de reenvío de paquetes, control de errores y garantizar el orden de los mensajes. Con TCP, estas funciones vienen integradas, lo que facilita el desarrollo y mantenimiento del sistema.

### Especificaciónes del protocolo

El protocolo desarrollado de nombre "Mychat protocol" esta pensado para proveer y definir como seran las interacciònes entre nuestro cliente y el servidor central concurrente que desarrollamos.

#### Estructura de los mensajes

Para lograr el equipo definiò los siguientes mensajes para el protocolo:
1. SYNC : Nombre de usuario : 0 : Socket del servidor : END
2. CONN : Nombre de destinatario : Mensaje a enviar : END
3. DCON : Nombre de usuario : Socket del servidor : Nombre del usuario con el que se establecio la conexion : flag : END

Considerar que la "flag" que se envia a la hora de la desconexion es necesaria para que el programa identifique cuando es una desconexion total del chat o si es una finalizaciòn de un chat que estaba siendo llevado a cabo.

#### Definiciòn de la interface del protocolo

- get_server_info(char port): Encargado de proveer a el servidor con las estructuras necesarias para abrir el STREAM Socket.

- get_client_info(char port): Encargado de proveer a el cliente con las estructuras necesarias para conectarse a el STREAM Socket

- init_socket_server(struct addrinfo *serv_info): Encargado de devolverle a el servidor un STREAM Socket listo para empezar a aceptar conexiònes (BIND y especificaciòn de las opciones).

- init_socket_client(struct addrinfo *serv_info): Encargado de devolverle a el cliente un STREAM Socket el cual ya esta listo para comunicarse con el servidor (Puerto y direcciòn IPv4 del servidor son requeridas).

- thread_listen(void *args): Funciòn principal que sera consumida por los hilos que cree el servidor a la hora de aceptar alguna conexiòn de un cliente, incluye la desencapsulaciòn de los mensajes que envie el cliente atraves del protocolo.

- sync_client(char user_name, int server_socket): Encargada de ensamblar el mensaje SYNC y enviarselo al thread asignado por el servidor para que pueda registar al usuario en la estructura de datos del servidor.

- con_client(char user_connect, int server_socket,char client_message): Encargado de ensamblar el mensaje CONN y enviarselo al thread asignado por el servidor para que este pueda redirigir el mensaje al usuario destinatario.

- dcon_client(char user, int server_socket, char user_connect,int flag): Encargado de ensamblar el mensaje DCON y enviarselo al thread asignado por el servidor para que elimine la entrada del usuario en la estructura de datos y cierre el socket de ese usuario.

#### Estructuras de datos utilizadas

La principal estructura de datos que utilizamos esta en el servidor y su proposito es guardar pares (Nombre,objeto) donde el objeto almacena informaciòn de cada usuario de la siguiente forma:
- Disponibilidad.
- Socket asignado.

Los metodos que modifiquen esta estructura debe de estar protegidos por medidas de concurrencia como LOCKS para evitar "race conditions" y errores a la hora de que varios threads la manipulen.

### Fases de la comunicación
EMANUEL
### Ejemplo
EMANUEL
### Diagramas
#### Secuencia
EMANUEL
#### Maquina de estado finito
##### Servidor
[MEF Servidor.pdf](https://github.com/user-attachments/files/17346056/MEF.Servidor.pdf)
##### Cliente
[MEF Cliente.pdf](https://github.com/user-attachments/files/17346059/MEF.Cliente.pdf)

## Aspectos logrados y no logrados
PINEDA

## Conclusiones
Primeramente podemos decir que el proyecto logró cumplir con la mayoría de los requisitos establecidos al inicio por el docente. Se implementaron con éxito los componentes clave del sistema, como la creación del servidor y cliente, la conexión entre ellos, la recepción y envío de mensajes, el manejo de múltiples clientes de forma concurrente, y un sistema básico de manejo de errores. Además, implementamos nuestro propio protocolo que a través de diferentes fases permitió la comunicación de la mejor manera entre los clientes varias fases del protocolo, permitiendo una comunicación estable entre los clientes.

En términos generales concluimos que este tipo de trabajos nos permiten seguir desarrollando nuestras habilidades como ingenieros y pudimos retarnos para comprender de la mejor manera como funcionan este tipo de protocolos como TCP en redes que usamos día a día, además recalcar la importancia de tener clara la teoría y real funcionamiento de lo que queremos crear como lo visto en clase para poder realizar la mejor abstracción posible y poder ingeniar desde los fundamentos.
## Referencias
https://beej.us/guide/bgnet/

https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/
