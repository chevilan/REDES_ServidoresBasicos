#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
//#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>


#define MAX 50
#define peticiones 5


int main(int argc, char **argv){


   int socketCliente;
   struct sockaddr_in dirCliente;
   socklen_t tamano;
   char mensaje[MAX];


   socketCliente = socket(AF_INET, SOCK_STREAM, 0);
   if(socketCliente < 0){
       perror("\nNo se pudo crear el socket\n");
       exit(EXIT_FAILURE);
   }


  


   return 0;
}
