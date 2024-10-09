# Mychat-2024
## Introducción

En el contexto actual de la tecnología de la información, la comunicación en tiempo real entre usuarios es esencial. Este proyecto desarrolló una aplicación de chat utilizando el modelo cliente/servidor, donde se implementó un protocolo de capa de aplicación que facilitó la interacción entre múltiples clientes a través de un servidor central. La aplicación de chat es capaz de gestionar la comunicación en tiempo real, permitiendo que los usuarios envíen y reciban mensajes de manera eficiente y efectiva. A través de este desarrollo, se abordó el diseño y la implementación de aplicaciones concurrentes en red. Para ello, se empleó la API Sockets de Berkley, lo que permitió establecer un protocolo de comunicaciones que facilitó la interacción entre la aplicación cliente y la aplicación servidor.

## Desarrollo

### Contexto y decisión de implementación

Antes de entrar en detalles, debemos aclarar el porque se implementaron "STREAM Sockets" antes que "DGRAM Sockets", el debate se presenta ya que son los protocolos de capa de transporte que gestionará la transmisión de datos entre la aplicación. Por un lado TCP es un protocolo orientado a la conexión, que establece una conexión estable entre dos puntos antes de transmitir datos. Proporciona entrega garantizada, orden de los datos y corrección de errores. Esto lo hace adecuado para aplicaciones donde la fiabilidad es crítica. En contraparte UDP no es orientado a la conexión, implemente envía datagramas sin asegurarse de que lleguen a su destino o en el orden correcto. Es más rápido pero no garantiza la entrega de los datos, siendo útil en aplicaciones donde la pérdida ocasional de paquetes es aceptable (Ejemplos claros los video juegos o plataformas de video en vivo).

Ahora si partimos de que cada tipo respeta las funcionalidades mencionadas anteriormente de acuerdo a cada protocolo, decidimos que el uso de Stream Socket (TCP) es más apropiado porque:

1. **Orientación a la Conexión**: Los Stream Sockets (TCP) aseguran que exista una conexión estable antes de enviar o recibir cualquier mensaje. Esto garantiza que ambos clientes estén conectados de manera segura, lo cual es crucial en un chat donde los usuarios deben saber que sus mensajes serán entregados.

2. **Entrega Garantizada**: En un chat, es vital que cada mensaje enviado por un usuario llegue a su destinatario sin pérdida. TCP se encarga de retransmitir cualquier mensaje perdido, mientras que con UDP, los mensajes podrían perderse sin que el usuario se dé cuenta, lo que afectaría negativamente la experiencia de la conversación.

3. **Integridad y Orden de los Mensajes**: Con TCP, los mensajes se entregan en el mismo orden en el que fueron enviados, lo cual es esencial para mantener la coherencia de una conversación. UDP, por su naturaleza, no garantiza el orden, lo que podría desorganizar los mensajes y hacer confusa la comunicación entre los usuarios.

4. **Congestión de Errores y Control**: TCP tiene mecanismos para la detección de errores y ajuste dinámico de la tasa de envío en función de la congestión de la red. Esto permite que los mensajes sean transmitidos de manera confiable, incluso en redes con fluctuaciones. Esto es crucial para asegurar que la experiencia de chat sea fluida y sin interrupciones, algo que no se podría garantizar con UDP.

5. **Evitar Complejidad Adicional en la Implementación**: El uso de Stream Sockets (TCP) simplifica el desarrollo de la aplicación. Al utilizar UDP, tendríamos que implementar manualmente funciones de reenvío de paquetes, control de errores y garantizar el orden de los mensajes. Con TCP, estas funciones vienen integradas, lo que facilita el desarrollo y mantenimiento del sistema.

### Especificaciónes del protocolo
MIGUEL
### Fases de la comunicación
EMANUEL
### Ejemplo
EMANUEL
### Diagramas
#### Secuencia
EMANUEL
#### Maquina de estado finito
MIGUEL
## Aspectos logrados y no logrados 
Después de haber realizado el proyecto satisfactoriamente podemos hacer retrospección de que realmente logramos y que no, para saber qué ítems se hicieron valoraremos los requisitos funcionales (que se pueden observar en el Backlog) planteados al inicio del proyecto según las directrices del docente.

Items logrados:

Crear servidor: Se desarrolló el servidor que aceptó conexiones de clientes. El servidor pudo iniciar, aceptar conexiones y manejar clientes conectados simultáneamente.

Crear cliente: Se implementó la aplicación cliente que permitió conectarse al servidor.El cliente se inició correctamente en modo consola y pudo ejecutarse sin errores.

Conectar cliente-servidor: Se implementó la capacidad del cliente para conectarse al servidor a través de sockets.El cliente se conectó con éxito al servidor y la conexión se estableció sin errores.

Permitir recibir mensajes del cliente al servidor: El servidor fue capaz de recibir mensajes enviados por los clientes conectados. El servidor recibió y almacenó correctamente los mensajes de los clientes sin pérdida de datos.

Habilitar la respuesta del servidor al cliente: El servidor envía mensajes de respuesta a los clientes conectados. Los mensajes del servidor fueron recibidos correctamente por los clientes sin errores de transmisión.

Soportar Concurrencia: El servidor manejó múltiples conexiones concurrentes de clientes sin interferencias.El servidor mantuvo al menos 2 clientes conectados simultáneamente y los mensajes no se mezclaron entre clientes.

Manejo de errores: Se implementaron mecanismos de detección y manejo de errores para garantizar la estabilidad de la conexión. El sistema identificó errores de red o de conexión y gestionó las desconexiones sin afectar a otros clientes.

Fase 1 protocolo: Logramos mediante el protocolo establecer conexión adecuadamente respetando los pasos establecidos. Se envió correctamente mensaje SYNC con la información pedida.

Fase 2 protocolo: Permitimos la comunicación entre los clientes respetando los procesos de protocolos establecidos. Así encapsulando los mensajes con el formato CON prescrito.

Fase 3 protocolo: Pudimos realizar el manejo de finalizar conexión con el protocolo cuando el cliente mandaba exit_ notificando con mensaje DCON.

Nota: de esta gran lista podemos de manera general denotar los puntos principales logrados de manera exitosa.

Items no logrados:
Autenticar cliente: No pudimos implementar un sistema de autenticación para verificar la identidad del cliente al conectarse al servidor.

Nota: no se pudo lograr este requisito por cuestiones de tiempo ya que tuvimos que priorizar otros aspectos tanto de código como de documentación, de igual forma era un requisito de prioridad 3 que realmente no era tan determinante ya que la seguridad no fue un requisito del proyecto.


## Conclusiones
  Primeramente podemos decir que el proyecto logró cumplir con la mayoría de los requisitos establecidos al inicio por el docente. Se implementaron con éxito los componentes clave del sistema, como la creación del servidor y cliente, la conexión entre ellos, la recepción y envío de mensajes, el manejo de múltiples clientes de forma concurrente, y un sistema básico de manejo de errores. Además, implementamos nuestro propio protocolo que a través de diferentes fases permitió la comunicación de la mejor manera entre los clientes varias fases del protocolo, permitiendo una comunicación estable entre los clientes.

  En términos generales concluimos que este tipo de trabajos nos permiten seguir desarrollando nuestras habilidades como ingenieros y pudimos retarnos para comprender de la mejor manera como funcionan este tipo de protocolos como TCP en redes que usamos día a día, además recalcar la importancia de tener clara la teoría y real funcionamiento de lo que queremos crear como lo visto en clase para poder realizar la mejor abstracción posible y poder ingeniar desde los fundamentos. 

## Referencias
https://beej.us/guide/bgnet/

https://www.geeksforgeeks.org/tcp-server-client-implementation-in-c/

