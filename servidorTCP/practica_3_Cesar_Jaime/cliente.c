#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
//#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#define MAX 100
#define peticiones 5


int main(int argc, char **argv){
    char ipservidor[MAX] = "127.0.0.1"; // Ip y puerto por defecto
    int numpuerto = 8080;
    if(argc > 2){
        strcpy(ipservidor,argv[1]);
        numpuerto = atoi(argv[2]);
    }
    int sockservidor;
    struct sockaddr_in dirservidor;
    //socklen_t tamano;
    char mensaje[MAX];


    sockservidor = socket(AF_INET, SOCK_STREAM, 0); //creamos el socket del servidor
    if(sockservidor < 0){ //si es menor que 0 (-1) da error
        perror("Error al abrir el socket");
        exit(EXIT_FAILURE);
    }
    if (inet_pton(AF_INET, ipservidor, &dirservidor.sin_addr) <= 0) {
        perror("inet_pton error");
        exit(EXIT_FAILURE);}
    // Inicializar direccionserv
    dirservidor.sin_family = AF_INET; // familia de direcciones//cualquier ip 
    dirservidor.sin_port = htons(numpuerto); // puerto del servidor
    

    if (connect(sockservidor, (struct sockaddr *) &dirservidor, sizeof(dirservidor)) < 0){
        perror("\nNo se pudo conectar al servidor\n");
        exit(EXIT_FAILURE);
    }
    // sleep(3);
    int n;
    while((n=recv(sockservidor, mensaje,50,0)) > 0){
            printf("\nMensaje recibido: %s\n", mensaje);
    }
    close(sockservidor);

   return 0;
}
