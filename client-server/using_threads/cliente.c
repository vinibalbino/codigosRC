// Este cliente usa o sistema fork() para gerar os processos filhos.
// Executar usando um parâremtro que indique a quantidade de filhos
// Exemplo ./client 5

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h> // Função close
#include <sys/wait.h> // Função Wait
#include <arpa/inet.h> // Inet_addr

void funcao_filho(int numFilho);

int main(int argc, char** argv){
    int nfilhos = 1;
    int pid; // Identificador do processo;
    int x; //Usaremos no fork
    if(argc >1){
        nfilhos = atoi(argv[1]);
    }

    for(x=0; x<nfilhos; x++){
        if((pid=fork()) == 0){
            funcao_filho(x+1);
            exit(0);
        }
    }// Fim For
    wait(NULL); // Os processos pais aguardam a criação de todos os filhos
}// Fim Main


void funcao_filho(int numFilho){ // Criação da funçao filho para se conectar ao servidor.
    int sock;
    struct sockaddr_in sAddr;
    char buffer[25];
    int x;


    sAddr.sin_family = AF_INET;
    sAddr.sin_addr.s_addr = INADDR_ANY;
    sAddr.sin_port = 0;

    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(sock, (const struct sockaddr *)&sAddr, sizeof(sAddr));

    sAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    sAddr.sin_port = htons(1972);

    if(connect(sock, (const struct sockaddr *)&sAddr, sizeof(sAddr))!=0){
        perror("Cliente!");
        return;
    }

    snprintf(buffer, 128, "dados do Cliente #%i.", numFilho);
    sleep(1); // Essa pausa ajuda na espera da conexão e desconexão dos clientes
    printf("Filho #%i. mandou %i caracteres\n", numFilho, send(sock, buffer, strlen(buffer), 0));
    sleep(1);
    printf("Filho #%i. recebeu %i caracteres\n", numFilho, recv(sock, buffer, 25, 0));
    sleep(1);

    close(sock);
};