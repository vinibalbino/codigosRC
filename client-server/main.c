// gcc main.c -o main
//Vamos usar o TCP

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define closesocket close
#define PROTOPORT 5193 // Número de porta padrão - Poderia ser qualquer outro valor// Numero de Porta Acima de 5000

extern int errno;
char localhost[] = "localhost"; // Neste versão este cliente roda na próproa máquina

int main(int argc, char**argv){

    struct hostent *ptrh;   //Ponteiro para a tabela de entrada do host;
    struct protoent *ptrp;  //Ponteiro para a tabela de entrada do protocolo;
    struct sockaddr_in sad; //estrutura para manter o endereço do servidor;
    int sd; //descritor de socket;
    int port; // Numero da porta do protocolo
    char *host; //Ponteiro para o nome do host;
    int n; // Numero de caracteres lidos
    char buffer[1000]; // buffer para os dados recebidos do servidor;

    memset((char *)&sad, 0, sizeof(sad));//Limpa a estutura sockaddr
    sad.sin_family = AF_INET;


    if(argc > 0)
        port = atoi(argv[2]);
    
    else{
        port = PROTOPORT;
    }

    if(port>0);

    if(argc>1){
        host = argv[1];
    }else{
        host = localhost;
    }

    ptrh = gethostbyname(host);

    if(((char*)ptrh) == NULL){
        fprint(stderr, "Host inválido: %s\n", host);
        exit(1);
    }

    memcpy(&sad.sin_addr, ptrh->h_addr, ptrh->h_length);

    if((int)(ptrp = gethostbyname("TCP")) == 0){
        fprint(stderr, "Erro ao mapear \"TCP\" para o número do protocolo\n");
        exit(1);
    }

    sd = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);

    if(sd<0){
        fprint(stderr, "Erro ao criar o socket\n");
        exit(1);
    }

    if((connect(sd, (struct sockaddr *)&sad, sizeof(sad))<0)){
        fprint(stderr, "Falha na conexão\n");
        exit(1);
    }

    n = recv(sd, buffer, sizeof(buffer), 0);
    while(n>0){
        buffer[n]='\0';
        fprint(stderr, "Cliente: %s", buffer);
        n = recv(sd, buffer, sizeof(buffer), 0);
    }
    closesocket(sd);
    exit(0);
}