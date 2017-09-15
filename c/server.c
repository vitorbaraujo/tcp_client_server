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

  char buffer[512] = {0};
  struct sockaddr_in server_address;
  int server, client, read_v, addr_length = sizeof(server_address);

  if((server = socket(AF_INET, SOCK_STREAM, 0)) == 0){
    printf("Error on socket\n");
    exit(1);
  }

  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(atoi(argv[1]));
  server_address.sin_addr.s_addr = INADDR_ANY;
  memset(server_address.sin_zero, '\0', sizeof(server_address.sin_zero));

  if(bind(server, (struct sockaddr *) &server_address, sizeof(server_address))){
    printf("Error on bind\n");
    exit(1);
  }

  if(listen(server, 1) < 0){
    printf("Error on listening\n");
    exit(1);
  }

  if((client = accept(server, (struct sockaddr *)&server_address, (socklen_t*) &addr_length)) < 0){
    printf("Error accepting\n");
    exit(1);
  }

  while(1){
    do{
      read_v = read(client, buffer, 512);
      printf("received [%s]\n", buffer);
    }while(read_v == 0);
    printf("%s\n", buffer);
    char* bla = "lala";
    send(client, bla, strlen(bla), 0);
  }

  return 0;
}
