/*

FORK()

-> Processos
-> PID Identificador do processo

            Copia do servidor pai
servidor Pai - |
               |-> Servidor Filho 1           <-| Cliente 1
               |-> Servidor Filho 2           <-| Cliente 2
               |-> Servidor Filho 3           <-| Cliente 3
              

              Quando um servidor filho cai, o cliente que está conectado ao mesmo 
              o clientes vira um  se tornam processos zombies
*/
/*
    Criar tipo um apache da vida
*/
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h> // wait e signal são usadas para previnir o surgimento de
#include <signal.h> // Zombies
#include <stdlib.h>
#include <unistd.h> // Função close

// Numero do sinal
// O sistema operacional realiza essa função
// Apenas a declaração
// Serve para aguardar o processo zombie caso apareça e pede para esperar
void sigchld_handler(int signo){

    while(waitpid(-1, NULL, WNOHANG)>0);
}

int main(int argc, char**argv){
    struct sockaddr_in sAddr;
    int listensock;
    int novosock;
    char buffer[25];
    int resultado;
    int n_lidos;
    int pid;
    int valor;


    listensock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    valor = 1;
    resultado = setsockopt(listensock, SOL_SOCKET, SO_REUSEADDR, &valor, sizeof(valor));

    if(resultado < 0){
        perror("error_setsockopt");
        return 1;
    }

    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(1972);
    sAddr.sin_addr.s_addr = INADDR_ANY;

    resultado = bind(listensock, (struct sockaddr *)&sAddr, sizeof(sAddr));
    
    if(resultado < 0){
        perror("erro_bind");
        return 1;
    }

    resultado = listen(listensock, 10);
    if(resultado < 0){
        perror("erro_listen");
        return 1;
    }

    signal(SIGCHLD, sigchld_handler); // Antes da criação, inicializamos o manipulador de sinais

    while(1){
        novosock = accept(listensock, NULL, NULL); // Lembrar que todos os descritores
        // São copiados do processo pai para o filho.
        
        // Para ver se o primeiro processo já foi criado
        if((pid = fork()) == 0){
            printf("processo filho %i, criado.\n", getpid());
            // Listensock e o servidor pai
            // novosock e o servidor filho
            close(listensock);
            n_lidos = recv(novosock, buffer, 25, 0);
            buffer[n_lidos] = '\0';
            printf("%s\n", buffer);
            send(novosock, buffer, n_lidos, 0);
            close(novosock);
            printf("Processo filho %i finalizado.\n", getpid());
            exit(0);
        }// Fim do if
    }// Fim while
}//Fim Main


