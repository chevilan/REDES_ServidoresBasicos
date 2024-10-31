#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define peticiones 5
#define MAX 100

int main(int argc, char const *argv[])
{

    int num_puerto = 8080; // Por defecto se asigna el puerto 8080
    if(argc > 1){
        num_puerto = atoi(argv[1]); // Si se introduce el puerto por linea de comandos se asigna ese puerto
    }
    // Declaración de variables

    int sockfd;
    struct sockaddr_in direccion_local, direccion_remota; //direccion local y direccion remota
    socklen_t longitud_direccion_remota = sizeof(direccion_remota); //tamano direccion remota
    char buffer[MAX]; //buffer maximo para recibir mensaje

    // Crear el socket UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error al crear el socket");
        exit(EXIT_FAILURE);
    }

    // Configurar la dirección local
    //memset(&direccion_local, 0, sizeof(direccion_local));
    direccion_local.sin_family = AF_INET;
    direccion_local.sin_port = htons(num_puerto);
    direccion_local.sin_addr.s_addr = INADDR_ANY;

    // Enlazar el socket a la dirección local
    if (bind(sockfd, (struct sockaddr *)&direccion_local, sizeof(direccion_local)) < 0) {
        perror("Error al enlazar el socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Recibir el mensaje
    int n = recvfrom(sockfd, buffer, MAX, 0, (struct sockaddr *)&direccion_remota, &longitud_direccion_remota);
    if (n < 0) {
        perror("Error al recibir el mensaje");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    buffer[n] = '\0';
    printf("Mensaje recibido: %s\n", buffer);
    char ip_str[INET_ADDRSTRLEN];
    if (inet_ntop(AF_INET, &(direccion_remota.sin_addr), ip_str, INET_ADDRSTRLEN) == NULL) {
        perror("inet_ntop error");
    }   
    int puerto = ntohs(direccion_remota.sin_port);
    printf("Recibido de:\n IP:%s PUERTO:%d \n",ip_str,puerto);

    close(sockfd);
    return 0;
}
