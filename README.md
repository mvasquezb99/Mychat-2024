# Mychat-2024
## Introducción

En el contexto actual de la tecnología de la información, la comunicación en tiempo real entre usuarios es esencial. Este proyecto desarrolló una aplicación de chat utilizando el modelo cliente/servidor, donde se implementó un protocolo de capa de aplicación que facilitó la interacción entre múltiples clientes a través de un servidor central. La aplicación de chat es capaz de gestionar la comunicación en tiempo real, permitiendo que los usuarios envíen y reciban mensajes de manera eficiente y efectiva. A través de este desarrollo, se abordó el diseño y la implementación de aplicaciones concurrentes en red. Para ello, se empleó la API Sockets de Berkley, lo que permitió establecer un protocolo de comunicaciones que facilitó la interacción entre la aplicación cliente y la aplicación servidor.

## Desarrollo

A la hora de realizar el proyecto, el equipo decidió utilizar la interfaz "STREAM Sockets", la cual esta soportada bajo el protocolo TCP de la capa de transporte. Antes y durante el desarrollo encontramos las siguientes razones por las cuales era mas conveniente utilizar esta interfaz en comparación a su equivalente soportado sobre UDP "DGRAM Sockets":

1. Orientación a la Conexión: Los Stream Sockets (TCP) aseguran que exista una conexión estable antes de enviar o recibir cualquier mensaje. Esto garantiza que ambos clientes estén conectados de manera segura, lo cual es crucial en un chat donde los usuarios deben saber que sus mensajes serán entregados.

2. Entrega Garantizada: En un chat, es vital que cada mensaje enviado por un usuario llegue a su destinatario sin pérdida. TCP se encarga de retransmitir cualquier mensaje perdido, mientras que con UDP, los mensajes podrían perderse sin que el usuario se dé cuenta, lo que afectaría negativamente la experiencia de la conversación.

3. Integridad y Orden de los Mensajes: Con TCP, los mensajes se entregan en el mismo orden en el que fueron enviados, lo cual es esencial para mantener la coherencia de una conversación. UDP, por su naturaleza, no garantiza el orden, lo que podría desorganizar los mensajes y hacer confusa la comunicación entre los usuarios.

4. Congestión de Errores y Control: TCP tiene mecanismos para la detección de errores y ajuste dinámico de la tasa de envío en función de la congestión de la red. Esto permite que los mensajes sean transmitidos de manera confiable, incluso en redes con fluctuaciones. Esto es crucial para asegurar que la experiencia de chat sea fluida y sin interrupciones, algo que no se podría garantizar con UDP.

5. Evitar Complejidad Adicional en la Implementación: El uso de Stream Sockets (TCP) simplifica el desarrollo de la aplicación. Al utilizar UDP, tendríamos que implementar manualmente funciones de reenvío de paquetes, control de errores y garantizar el orden de los mensajes. Con TCP, estas funciones vienen integradas, lo que facilita el desarrollo y mantenimiento del sistema.
6. 

## Aspectos logrados y no logrados 
PINEDA

## Conclusiones

## Referencias

