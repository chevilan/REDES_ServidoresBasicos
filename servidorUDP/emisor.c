#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#define MAX 100
#define peticiones 5


int main(int argc, char **argv){
    char ip_emisor[MAX] = "127.0.0.1"; // Ip y puerto por defecto
    int num_puerto = 8080;
    if(argc > 2){
        strcpy(ip_emisor,argv[1]);
        num_puerto = atoi(argv[2]);
    }   

    int socket_receptor; // Identificador del socket del receptor
    struct sockaddr_in direccion_receptor; // Dirección del socket del receptor
    socklen_t tamano=sizeof(direccion_receptor);
    char mensaje[MAX] = "Mensaje enviado por el emisor!\n";

    socket_receptor=socket(AF_INET,SOCK_DGRAM,0); // abre el socket
    if (socket_receptor<0){
        perror("Error al abrir el socket del emisor\n"); // si da error
        exit(EXIT_FAILURE);
    }

    if (inet_pton(AF_INET, ip_emisor, &direccion_receptor.sin_addr) <= 0) { //asignar la ip(de format pre a network) a la direccion del receptor
        perror("inet_pton error");
        exit(EXIT_FAILURE);
    }
    direccion_receptor.sin_family = AF_INET; // familia de direcciones
    direccion_receptor.sin_port = htons(num_puerto); // puerto del servidor
    //enviar al socket del receptor, el mensaje de tamaño mensaje, direccion de receptor, tamaño
    if (sendto(socket_receptor, mensaje, strlen(mensaje), 0, (struct sockaddr *)&direccion_receptor, tamano) < 0) {
        perror("Error al enviar el mensaje");
        close(socket_receptor);
        exit(EXIT_FAILURE);
    }
    // cuando enfia el mensaje imprime que ha enviado el mensaje
    printf("Mensaje enviado: %s\n", mensaje);
    close(socket_receptor);
    return 0;

}
