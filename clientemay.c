#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <ctype.h>

#define MAX 100
#define LINEAMAX 1000
#define peticiones 5

void mayus(char* cadena);

int main(int argc, char **argv){


    char filename[MAX] = "archivo.txt";
    char lineaIN[LINEAMAX], lineaOUT[LINEAMAX];
    char ipservidor[MAX] = "127.0.0.1";
    int numpuerto = 8080;
    if(argc > 3){
        strncpy(filename, argv[1], MAX - 1);
        strncpy(ipservidor, argv[2], MAX - 1);
        numpuerto = atoi(argv[3]);
    }


    FILE *fileIN = fopen(filename, "r");
    if (fileIN == NULL) {
        perror("Error al abrir el archivo de entrada");
        exit(EXIT_FAILURE);
    }


    mayus(filename);
    FILE *fileOUT = fopen(filename, "w+");
    if (fileOUT == NULL) {
        perror("Error al abrir el archivo de salida");
        fclose(fileIN);
        exit(EXIT_FAILURE);
    }


    int socketServidor;
    struct sockaddr_in dirServidor;
    ssize_t n;
    if ((socketServidor = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("Error al abrir el socket");
        fclose(fileIN);
        fclose(fileOUT);
        exit(EXIT_FAILURE);
    }


    memset(&dirServidor, 0, sizeof(dirServidor));
    dirServidor.sin_family = AF_INET;
    dirServidor.sin_port = htons(numpuerto);


    if (inet_pton(AF_INET, ipservidor, &dirServidor.sin_addr) <= 0){
        perror("inet_pton error");
        fclose(fileIN);
        fclose(fileOUT);
        close(socketServidor);
        exit(EXIT_FAILURE);
    }


    if (connect(socketServidor, (struct sockaddr *) &dirServidor, sizeof(dirServidor)) < 0){
        perror("No se pudo conectar al servidor");
        fclose(fileIN);
        fclose(fileOUT);
        close(socketServidor);
        exit(EXIT_FAILURE);
    }


    printf("Conectado al servidor %s en el puerto %d\n", ipservidor, numpuerto);


   // bucle
   while(fgets(lineaIN, LINEAMAX, fileIN)){    // lee linea
        printf("Leyendo línea: %s", lineaIN);
        sleep(3);   

        // envia linea al servidor
        if(send(socketServidor, lineaIN, strlen(lineaIN), 0) < 0){
            perror("No se pudo enviar el mensaje");
            fclose(fileIN);
            fclose(fileOUT);
            close(socketServidor);
            exit(EXIT_FAILURE);
                    }
        printf("\nLínea enviada: %s", lineaIN);


        // recibe linea y escribe file
        n = recv(socketServidor, lineaOUT, LINEAMAX, 0);
        if (n < 0) {
            perror("No se pudo recibir el mensaje");
            fclose(fileIN);
            fclose(fileOUT);
            close(socketServidor);
            exit(EXIT_FAILURE);
        } else if (n == 0) {
            printf("Conexión cerrada por el servidor\n");
            break; // conexión cerrada por el servidor
        }
        lineaOUT[n] = '\0'; // asegurarse de que la cadena esté terminada en nulo
        printf("\nLínea recibida: %s", lineaOUT);
        fprintf(fileOUT, "%s", lineaOUT);
    }
    printf("\n");
    close(socketServidor);
    fclose(fileIN);
    fclose(fileOUT);


    return 0;
}

void mayus(char* cadena) {
    for (int i = 0; cadena[i] != '\0'; i++) {
        cadena[i] = toupper(cadena[i]);
    }
}