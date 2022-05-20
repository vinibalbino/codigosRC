#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h> // Função close
#include <sys/wait.h> // Função Wait
#include <arpa/inet.h> // Inet_addr
#include <sys/ioctl.h>



int main(int argc, char** argv){

    struct sockaddr_in sAddr;
    fd_set readset, testset;
    int listensock;
    int novosock;
    char buffer[25];
    int resultado;
    int nread;
    int x, val;

    listensock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    val=1;
    
    // Geralmente, o linux mantém a porta e oedereço reservado ao programa. SO_REUSEADDR evita isso.

    resultado = setsockopt(listensock, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
    if(resultado < 0){
        perror("Servidor");
        return 0;
    }

    sAddr.sin_family = AF_INET;
    sAddr.sin_port = htons(1972);
    sAddr.sin_addr.s_addr = INADDR_ANY;
    // INADDR_ANY indica que serão ouvidos qualquer ip


    resultado = bind(listensock, (struct sockaddr *)&sAddr, sizeof(sAddr));
    if(resultado < 0){
        perror("Bind");
        return 0;
    }

    resultado = listen(listensock, 5);

    if(resultado < 0){
        perror("listen");
        return 0;
    }

    FD_ZERO(&readset); // Inicialização do descritor
    FD_SET(listensock, &readset);
    // A função FD_SET adiciona a funçao listen ao socket. Quando um cliente quiser
    // Se conectar o socket será avisado.

    while(1){
        testset = readset;
        // A função select modifica o conjunto passado. Uma variável mantém o estado
        // e a outra interage com a função select().
        resultado = select(FD_SETSIZE, &testset, NULL, NULL, NULL);
        if(resultado < 0){
            perror("Func Select");
            return 0;
        }
        for(x = 0; x<FD_SETSIZE; x++){
        //FD_SETSIZE, é um conjunto de constantes do kernel. o valor é 1024
            if(FD_ISSET(x, &testset)){
            // FD_ISSET é usado para determinar se o descritor possui atividade ou não.
            // Pra isso usa-se uma flag.
                if(x == listensock){
                    novosock = accept(listensock, NULL, NULL);
                    FD_SET(novosock, &readset);
                }else{
                    nread = recv(x, buffer, 25, 0);
                    if(nread <= 0){
                        close(x);
                        FD_CLR(x, &readset);
                        printf("Cliente do manipulador #%i desconectado\n", x);
                    }else{
                        buffer[nread] = '\0';
                        printf("%s\n", buffer);
                        send(x, buffer, nread, 0);
                    }
                }
            }
        }
    }
}