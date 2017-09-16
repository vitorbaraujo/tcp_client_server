#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

char *parse_op(char op){
    if(op == '+') return "add";
    else if(op == '-') return "sub";
    else if(op == '*') return "mul";
    else if(op == '/') return "div";

    return "inv";
}

req_pkt *parse_msg(char *input){
    char first_n[512], second_n[512], op;
    int frst_num_ptr = 0, sec_num_ptr = 0;
    req_pkt *requisition = malloc(sizeof(req_pkt));

    int op_found = 0;
    int input_size = strlen(input);

    for(int i=0; i<input_size; i++){
        if(isspace(input[i])) continue;

        if(isdigit(input[i])){
            if(op_found){
                second_n[sec_num_ptr++] = input[i];
            }
            else{
                first_n[frst_num_ptr++] = input[i];
            }
        }
        else{
            op = input[i];
            op_found = 1;
        }
    }

    first_n[frst_num_ptr] = '\0';
    second_n[sec_num_ptr] = '\0';

    char *operator = parse_op(op);

    if(strcmp(operator, "inv") == 0){
        strcpy(requisition->op, "inv");
        requisition->op1 = 0;
        requisition->op2 = 0;
        return requisition;
    }

    strcpy(requisition->op, operator);
    requisition->op1 = atoi(first_n);
    requisition->op2 = atoi(second_n);

    return requisition;
}

int main(int argc, char* argv[]){
  if(argc < 2){
    printf("Specify port\n");
    exit(1);
  }

  struct sockaddr_in client_address, server_address;
  int server, read_v;
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
  req_pkt *data_to_send = malloc(sizeof(req_pkt));
  answ_pkt *result = malloc(sizeof(answ_pkt));

  while(1){
      fgets(operacao, 512, stdin);
      data_to_send = parse_msg(operacao);
      send(server, data_to_send, sizeof(req_pkt), 0);
      do{
        read_v = read(server, result, sizeof(answ_pkt));
      }while(read_v == 0);
      if(result->invalid == 0){
        printf("Resultado: %d\n", result->value);
      }else{
        printf("Houve um erro no pedido de calculo\n");
      }
  }

  return 0;
}
