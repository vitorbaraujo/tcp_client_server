#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]){
  if(argc < 2){
    printf("Specify port\n");
    exit(1);
  }

  struct sockaddr_in client_address, server_address;
  int server, read_v;
  char buffer[512] = {0};
  char operacao[512] = {0};

  if((server = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    printf("Socket error\n");
    exit(1);
  }

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(atoi(argv[1]));
  server_address.sin_addr.s_addr = INADDR_ANY;
  memset(server_address.sin_zero, '\0', sizeof(server_address.sin_zero));

  if(connect(server, (struct sockaddr *) &server_address, sizeof(server_address)) < 0){
    printf("Error on connect\n");
    exit(1);
  }

  printf("Digite as operacoes:\n");
  while(1){
      fgets(operacao, 512, stdin);
      send(server, operacao, strlen(operacao), 0);
      do{
        read_v = read(server, buffer, 1024);
      }while(read_v == 0);
      printf("%s\n", buffer);
  }

  return 0;
}
