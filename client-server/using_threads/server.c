/*
 Multithreading ou servidor multithread
 Compilar : gcc nome_do_arquivo.c -o servidor -lpthread
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <pthread.h>

void* thread_proc(void* arg);

int main(int argc, char**argv){
    struct sockaddr_in sAddr;
    int listensock;
    int novosock;
    int resultado;
    pthread_t thread_id;
    int valor;

    listensock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    valor = 1;
    resultado = setsockopt(listensock, SOL_SOCKET, SO_REUSEADDR, &valor, sizeof(valor));
    if(resultado <0){
        perror("erro_setsockopt");
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

    while(1){
        
        novosock = accept(listensock, NULL, NULL);
        // Uma vez que o cliente conecta, uma nova thread é iniciada.
        // O descriptor para esse novo socket é passado para a função thread.
        // Após passado para a função, não há necessidade da thread pai fechar o descriptor 
        
    }
}//Fim Main

