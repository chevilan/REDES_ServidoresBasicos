#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define peticiones 5

int main(int argc, char const *argv[])
{

    int numpuerto = 8080; // Por defecto se asigna el puerto 8080
    if(argc > 1){
        numpuerto = atoi(argv[1]); // Si se introduce el puerto por linea de comandos se asigna ese puerto
    }
    // Declaración de variables
    int sockservidor, sockcliente; // Los sockets tanto del servidor como del cliente
    struct sockaddr_in direccionserv, direccioncliente; // Las direcciones del servidor y del cliente (struct sockaddr_in)
    socklen_t tamano; // El tamaño de la direccion del cliente
    char* mensaje1 = "Hola cliente! Este es el primer mensaje\n"; // Mensaje 1
    char* mensaje2 = "Este es el segundo mensaje...\n"; // Mensaje 2

    sockservidor = socket(AF_INET, SOCK_STREAM, 0); // Se crea el socket del servidor, AF_INET familia de direcciones,
    if(sockservidor < 0){ // Si es menor que 0 (-1) da error
        perror("Error al abrir el socket");
        exit(EXIT_FAILURE);
    }

    // Inicializar direccionserv
    direccionserv.sin_family = AF_INET; // Familia de direcciones
    direccionserv.sin_addr.s_addr = htonl(INADDR_ANY); // Cualquier ip
    direccionserv.sin_port = htons(numpuerto); // Puerto del servidor

    if(bind(sockservidor, (struct sockaddr *) &direccionserv, sizeof(direccionserv)) < 0){ // Asignamos la direccion al socket
        perror("No se pudo asignar direccion"); 
        exit(EXIT_FAILURE);
    }
    if(listen(sockservidor, peticiones) < 0){ // Escuchamos el socket
        perror("No se pudo escuchar el socket");
        exit(EXIT_FAILURE);
    }

    while(1){ //bucle infinito
        tamano = sizeof(direccioncliente);
        if((sockcliente = accept(sockservidor, (struct sockaddr *) &direccioncliente, &tamano)) < 0){ // Aceptamos la conexion
            perror("No se pudo aceptar la conexion");
            exit(EXIT_FAILURE);
        }

        char ip[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, &direccioncliente.sin_addr, ip, INET_ADDRSTRLEN) != NULL) {
            printf("IP del cliente: %s\n", ip);
        }
        printf("El puerto es %d\n", ntohs(direccioncliente.sin_port)); // Mostramos el puerto del cliente
        ssize_t enviados;
        enviados = send(sockcliente, mensaje1, strlen(mensaje1), 0);
        if(enviados < 0){
            perror("No se pudo enviar el mensaje");
            exit(EXIT_FAILURE);
        } 
        enviados=send(sockcliente, mensaje2, strlen(mensaje2), 0);
        if(enviados < 0){
            perror("No se pudo enviar el mensaje");
            exit(EXIT_FAILURE);
        } // Enviamos el mensaje
        // %zd es para mostrar el tamaño de un size_t
        printf("Numero de bytes enviados: %lu\n", strlen(mensaje1)+strlen(mensaje2)); // Mostramos el numero de bytes enviados

        close(sockcliente); // Cerramos el socket del cliente
    }

    close(sockservidor); // Cerramos el socket del servidor
    return 0;
}
