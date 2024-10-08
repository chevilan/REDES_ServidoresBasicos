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

    int numpuerto = 8080;
    if(argc > 1){
        numpuerto = atoi(argv[1]);
    }
    //declarar variables
    int sockservidor, sockcliente; //los sockets tanto del servidor como del cliente
    struct sockaddr_in direccionserv, direccioncliente; //las direcciones del servidor y del cliente (struct sockaddr_in)
    socklen_t tamano; //el tamaño de la direccion del cliente
    char* mensaje = "Hola cliente!\n\n"; //mensaje a enviar

    sockservidor = socket(AF_INET, SOCK_STREAM, 0); //creamos el socket del servidor
    if(sockservidor < 0){ //si es menor que 0 (-1) da error
        perror("Error al abrir el socket");
        exit(EXIT_FAILURE);
    }

    // Inicializar direccionserv
    direccionserv.sin_family = AF_INET; // familia de direcciones
    direccionserv.sin_addr.s_addr = htonl(INADDR_ANY); //cualquier ip
    direccionserv.sin_port = htons(numpuerto); // puerto del servidor

    if(bind(sockservidor, (struct sockaddr *) &direccionserv, sizeof(direccionserv)) < 0){ //asignamos la direccion al socket
        perror("No se pudo asignar direccion"); 
        exit(EXIT_FAILURE);
    }
    if(listen(sockservidor, peticiones) < 0){ //escuchamos el socket
        perror("No se pudo escuchar el socket");
        exit(EXIT_FAILURE);
    }

    while(1){ //bucle infinito
        tamano = sizeof(direccioncliente);
        if((sockcliente = accept(sockservidor, (struct sockaddr *) &direccioncliente, &tamano)) < 0){ //aceptamos la conexion
            perror("No se pudo aceptar la conexion");
            exit(EXIT_FAILURE);
        }

        char ip[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, &direccioncliente.sin_addr, ip, INET_ADDRSTRLEN) != NULL) {
            printf("IP del cliente: %s\n", ip);
        }
        printf("El puerto es %d\n", ntohs(direccioncliente.sin_port)); //mostramos el puerto del cliente

        if(send(sockcliente, mensaje, strlen(mensaje), 0) < 0){
            perror("No se pudo enviar el mensaje");
            exit(EXIT_FAILURE);
        } //enviamos el mensaje
        //%zd es para mostrar el tamaño de un size_t
        printf("Numero de bytes enviados: %lu\n", strlen(mensaje)); //mostramos el numero de bytes enviados

        close(sockcliente); //cerramos el socket del cliente
    }

    close(sockservidor); //cerramos el socket del servidor
    return 0;
}
