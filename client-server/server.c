#ifndef __linux__
#include <windows.h>
#include <winsock.h>
#else
#define closesocket close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define PROTOPORT 5193 // número de portas padrão
#define QLEN 6         // tamanho de pedidos de conexão que podem ser aceitos
int visits = 0;        // guardar o contador de conexões do meu meu servidor

int main(int argc, char **argv)
{
  struct hostent *ptrh;   // ponteiro para a entrada da tabela host
  struct protoent *ptrp;  // ponteiro para a entrada da tabela do protocolo
  struct sockaddr_in sad; // estrutura que armazena o endereço do servidor
  struct sockaddr_in cad; // estrutura que armazena o endereço do cliente
  int sd, sd2;            // descritor de socket
  int port;               // número de porta do protocolo
  int alen;               // tamanho do endereço que será passado
  char buffer[1000];      // buffer para armazenar as mensagens enviadas pelo servidor

  #ifdef _WIN32
    #define WSADATA wsaData;
    #define WSAstartup (0x0101, &wsaData);
  #endif

  memset((char *)&sad, 0, sizeof(sad)); // limpa a estrutura sokaddr;
  sad.sin_family = AF_INET;
  sad.sin_addr.s_addr = INADDR_ANY; // configura o endereo local

  if (argc > 1)
  {                       // verifica se o argumento de entrada é válido
    port = atoi(argv[1]); // converte o argumento para binário
  }
  else
  {
    port = PROTOPORT; // usa o número de porta padrão
  }
  if (((int)(ptrp = getprotobyname("TCP"))) == 0)
  {
    fprintf(stderr, "Falha ao mapear \"TCP\" para o numero de protocolo\n");
    exit(1);
  }

  // * Cria socket
  sd = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);
  if (sd < 0)
  {
    fprintf(stderr, "Falha ao criar socket\n");
    exit(1);
  }

  // * Fazer a linkagem (bind) do endereço local com o socket
  if (bind(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0)
  {
    fprintf(stderr, "Falha no bind\n");
    exit(1);
  }

  // * Especificar o tamanho da fila das mensagens de entrada
  if (listen(sd, QLEN < 0))
  {
    fprintf(stderr, "Falha no listen\n");
    exit(1);
  }

  while (1)
  {
    alen = sizeof(cad);
    fprintf(stderr, "SERVIDOR: Aguardando por conexoes...\n");

    if ((sd2 = accept(sd, (struct sockaddr *)&cad, &alen)) < 0)
    {
      fprintf(stderr, "Falha na função accept\n");
      exit(1);
    }
    visits++;
    sprintf(buffer, "Este servidor foi contactado %d vezes\n", visits, visits == 1 ? "." : "s.");
    fprintf(stderr, "SERVIDOR: %s", buffer);
    closesocket(sd2);
  }

  return 0;
}
