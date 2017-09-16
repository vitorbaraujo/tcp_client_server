#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

typedef struct req {
  char op[3];
  int op1;
  int op2;
} req_pkt;

typedef struct answ {
  int invalid;
  int value;
} answ_pkt;

answ_pkt *add(int a, int b){
  answ_pkt *result = malloc(sizeof(answ_pkt));
  result->invalid = 0;
  result->value = a+b;
  return result;
}

answ_pkt *sub(int a, int b){
  answ_pkt *result = malloc(sizeof(answ_pkt));
  result->invalid = 0;
  result->value = a-b;
  return result;
}

answ_pkt *mul(int a, int b){
  answ_pkt *result = malloc(sizeof(answ_pkt));
  result->invalid = 0;
  result->value = a*b;
  return result;
}

answ_pkt *divi(int a, int b){
  answ_pkt* result = malloc(sizeof(answ_pkt));
  if(!b){
    result->invalid = 1;
    return result;
  }

  result->invalid = 0;
  result->value = a/b;
  return result;
}

answ_pkt *calc(req_pkt *requisition){
  answ_pkt *answer = malloc(sizeof(answ_pkt));
  if(!(strcmp(requisition->op, "add"))){
    answer = add(requisition->op1, requisition->op2);
  }else if(!(strcmp(requisition->op, "sub"))){
    answer = sub(requisition->op1, requisition->op2);
  }else if(!(strcmp(requisition->op, "mul"))){
    answer = mul(requisition->op1, requisition->op2);
  }else if(!(strcmp(requisition->op, "div"))){
    answer = divi(requisition->op1, requisition->op2);
  }else{
    answer->invalid = 1;
  }

  return answer;
}

int main(int argc, char* argv[]){
  if(argc < 2){
    printf("Specify port\n");
    exit(1);
  }

  req_pkt *buffer = malloc(sizeof(req_pkt));
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

  answ_pkt* result = malloc(sizeof(answ_pkt));

  while(1){
    do{
      read_v = read(client, buffer, sizeof(req_pkt));
    }while(read_v == 0);

    result = calc(buffer);
    send(client, result, sizeof(answ_pkt), 0);
  }

  return 0;
}
