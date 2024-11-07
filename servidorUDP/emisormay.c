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

// Declaración de la función que convierte una cadena a mayúsculas
void mayus(char* cadena);

int main(int argc, char **argv) {
    // Declaración de variables
    char filename[MAX] = "archivo.txt"; // Nombre del archivo de entrada
    char lineaIN[LINEAMAX], lineaOUT[LINEAMAX]; // Buffers para leer y escribir líneas
    char ipservidor[MAX] = "127.0.0.1"; // Dirección IP del servidor
    int numpuerto = 8080; // Número de puerto del servidor

    // Si se pasan argumentos, se actualizan las variables con los valores proporcionados
    if (argc > 3) {
        strncpy(filename, argv[1], MAX - 1);
        strncpy(ipservidor, argv[2], MAX - 1);
        numpuerto = atoi(argv[3]);
    }

    // Abrir el archivo de entrada en modo lectura
    FILE *fileIN = fopen(filename, "r");
    if (fileIN == NULL) {
        perror("Error al abrir el archivo de entrada");
        exit(EXIT_FAILURE);
    }

    // Convertir el nombre del archivo a mayúsculas y abrir el archivo de salida en modo escritura
    mayus(filename);
    FILE *fileOUT = fopen(filename, "w+");
    if (fileOUT == NULL) {
        perror("Error al abrir el archivo de salida");
        fclose(fileIN);
        exit(EXIT_FAILURE);
    }

    // Declaración de variables para el socket
    int socketServidor;
    struct sockaddr_in dirServidor;
    ssize_t n;

    // Crear el socket
    if ((socketServidor = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Error al abrir el socket");
        fclose(fileIN);
        fclose(fileOUT);
        exit(EXIT_FAILURE);
    }

    // Inicializar la estructura de la dirección del servidor
    memset(&dirServidor, 0, sizeof(dirServidor));
    dirServidor.sin_family = AF_INET;
    dirServidor.sin_port = htons(numpuerto);

    // Convertir la dirección IP del servidor de texto a binario
    if (inet_pton(AF_INET, ipservidor, &dirServidor.sin_addr) <= 0) {
        perror("inet_pton error");
        fclose(fileIN);
        fclose(fileOUT);
        close(socketServidor);
        exit(EXIT_FAILURE);
    }

    // Bucle para leer líneas del archivo de entrada, enviarlas al servidor y escribir la respuesta en el archivo de salida
    while (fgets(lineaIN, LINEAMAX, fileIN)) { // Lee una línea del archivo de entrada
        printf("Leyendo línea: %s", lineaIN);
        sleep(3); // Espera 3 segundos

        // Enviar la línea al servidor
        if (sendto(socketServidor, lineaIN, strlen(lineaIN),0, (struct sockaddr *) &dirServidor,sizeof(dirServidor)) < 0) {
            perror("No se pudo enviar el mensaje");
            fclose(fileIN);
            fclose(fileOUT);
            close(socketServidor);
            exit(EXIT_FAILURE);
        }
        printf("\nLínea enviada: %s", lineaIN);

        // Recibir la respuesta del servidor
        socklen_t lenDirServidor = sizeof(dirServidor);
        n = recvfrom(socketServidor, lineaOUT, LINEAMAX, 0,(struct sockaddr *) &dirServidor,&lenDirServidor);
        if (n < 0) {
            perror("No se pudo recibir el mensaje");
            fclose(fileIN);
            fclose(fileOUT);
            close(socketServidor);
            exit(EXIT_FAILURE);
        } else if (n == 0) {
            printf("Conexión cerrada por el servidor\n");
            break; // Conexión cerrada por el servidor
        }
        lineaOUT[n] = '\0'; // Asegurarse de que la cadena esté terminada en nulo
        printf("\nLínea recibida: %s", lineaOUT);

        // Escribir la línea recibida en el archivo de salida
        fprintf(fileOUT, "%s", lineaOUT);
    }
    printf("\n");

    // Cerrar el socket y los archivos
    close(socketServidor);
    fclose(fileIN);
    fclose(fileOUT);

    return 0;
}

// Función que convierte una cadena a mayúsculas
void mayus(char* cadena) {
    for (int i = 0; cadena[i] != '\0'; i++) {
        cadena[i] = toupper(cadena[i]);
    }
}