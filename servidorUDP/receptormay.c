#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

#define MAX 100
#define LINEAMAX 1000

void mayus(char* cadena);

int main() {
    int sockfd;
    struct sockaddr_in direccion_local, direccion_remota;
    char buffer[MAX];
    socklen_t longitud_direccion_remota = sizeof(direccion_remota);
    ssize_t n;
    int num_puerto = 8080; // Puerto del servidor

    // Crear el socket UDP
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error al abrir el socket");
        exit(EXIT_FAILURE);
    }

    // Inicializar la estructura de la dirección local
    memset(&direccion_local, 0, sizeof(direccion_local));
    direccion_local.sin_family = AF_INET;
    direccion_local.sin_addr.s_addr = htonl(INADDR_ANY);
    direccion_local.sin_port = htons(num_puerto);

    // Enlazar el socket con la dirección local
    if (bind(sockfd, (struct sockaddr *) &direccion_local, sizeof(direccion_local)) < 0) {
        perror("Error al enlazar el socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Servidor UDP esperando mensajes en el puerto %d...\n", num_puerto);
    int flag=0;
    // Bucle para recibir mensajes y enviar respuestas
    while (1) {
        // Recibir un mensaje del cliente
        n = recvfrom(sockfd, buffer, MAX, 0, (struct sockaddr *) &direccion_remota, &longitud_direccion_remota);
        if (n < 0) {
            perror("No se pudo recibir el mensaje");
            close(sockfd);
            exit(EXIT_FAILURE);
        } else if (n == 0) {
            printf("\nConexión cerrada por el cliente\n");
            close(sockfd);
            break;
        }

        // Convertir la dirección IP a formato de presentación
        if(flag==0){
            char ip_str[INET_ADDRSTRLEN];
            if (inet_ntop(AF_INET, &(direccion_remota.sin_addr), ip_str, INET_ADDRSTRLEN) == NULL) {
                perror("inet_ntop error");
            } else {
                printf("IP del emisor: %s\n", ip_str);
            }

            // Convertir el puerto a formato legible
            int puerto = ntohs(direccion_remota.sin_port);
            printf("Puerto del emisor: %d\n", puerto);
            flag=1;
        }

        buffer[n] = '\0'; // Asegurarse de que la cadena esté terminada en nulo
        printf("\nLínea recibida: %s\n", buffer);
        // Convertir el mensaje a mayúsculas
        mayus(buffer);

        // Enviar el mensaje de vuelta al cliente
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *) &direccion_remota, longitud_direccion_remota) < 0) {
            perror("No se pudo enviar el mensaje");
            close(sockfd);
            break;
        }
        printf("Mensaje enviado de vuelta: %s\n", buffer);
    }

    // Cerrar el socket
    close(sockfd);
    return 0;
}

// Función que convierte una cadena a mayúsculas
void mayus(char* cadena) {
    for (int i = 0; cadena[i] != '\0'; i++) {
        cadena[i] = toupper(cadena[i]);
    }
}