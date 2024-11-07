#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#define MAX 100
#define LINEAMAX 1000
#define peticiones 5

void mayus(char* cadena);

int main(int argc, char **argv){


    char linea[LINEAMAX];
    int numpuerto = 8080;
    int socketServidor, socketCliente;
    struct sockaddr_in direccionserv, direccioncliente;
    socklen_t tamano;


    if((socketServidor = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Error al abrir el socket");
        exit(EXIT_FAILURE);
    }
    
    direccionserv.sin_family = AF_INET; // familia de direcciones
    direccionserv.sin_addr.s_addr = htonl(INADDR_ANY); //cualquier ip
    direccionserv.sin_port = htons(numpuerto); // puerto del servidor


    if(bind(socketServidor, (struct sockaddr *) &direccionserv, sizeof(direccionserv)) < 0){ //asignamos la direccion al socket
        perror("No se pudo asignar direccion");
        exit(EXIT_FAILURE);
    }
    if(listen(socketServidor, 5) < 0){ //escuchamos el socket
        perror("No se pudo escuchar el socket");
        exit(EXIT_FAILURE);
    }


    while(1){
        //acepta conexion y recibe linea
        tamano = sizeof(direccioncliente);
        if((socketCliente = accept(socketServidor, (struct sockaddr *) &direccioncliente, &tamano)) < 0){ //aceptamos la conexion
            perror("No se pudo aceptar la conexion");
            continue;
        }


        while(1) {
            ssize_t n = recv(socketCliente, linea, LINEAMAX, 0);
            if (n < 0) {
                perror("No se pudo recibir el mensaje");
                close(socketCliente);
                break;
            } else if (n == 0) {
                printf("\nConexión cerrada por el cliente\n");
                close(socketCliente);
                break;
            }


            linea[n] = '\0'; // asegurarse de que la cadena esté terminada en nulo
            printf("\nLínea recibida: %s", linea);


            mayus(linea); // convierte a mayusculas


            if(send(socketCliente, linea, strlen(linea), 0) < 0){
                perror("No se pudo enviar el mensaje");
                close(socketCliente);
                break;
            }
            printf("\nLínea enviada: %s", linea);
        }
    }
    printf("\n");
    close(socketServidor);


    return 0;
}


    
void mayus(char* cadena) {
    for (int i = 0; cadena[i] != '\0'; i++) {
        cadena[i] = toupper(cadena[i]);
    }
}