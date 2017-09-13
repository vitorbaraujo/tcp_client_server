#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>


int main(int argc, char* argv[]){
  if(argc < 3){
    printf("You need to specify IP and port\n");
    exit(1);
  }

  int sock;

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(atoi(argv[2]));
  server_addr.sin_addr.s_addr = inet_addr(argv[1]);
  memset(server_addr.sin_zero, '\0', sizeof(server_addr.sin_zero));

  sock = socket(AF_INET, SOCK_STREAM, 0);

  if(sock < 0){
    printf("Error on socket\n");
    exit(1);
  }

  if(connect(sock, (struct sockaddr *) &server_addr, sizeof(struct sockaddr)) < 0){
    printf("Error on connecting\n");
  }
  while(1){
    char* message = "1";
    send(sock, message, strlen(message), 0);
  }
  return 0;
}
