#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

char *parse_op(char op){
    if(op == '+') return "add";
    else if(op == '-') return "sub";
    else if(op == '*') return "mul";
    else if(op == '/') return "div";

    return "inv";
}

char *parse_msg(char *input){
    char first_n[512], second_n[512], op;
    int frst_num_ptr = 0, sec_num_ptr = 0;

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
        return "inv 0 0";
    }

    char *expression;

    expression = (char *)malloc(512 * sizeof(char));

    sprintf(expression, "%s %s %s", operator, first_n, second_n);

    return expression;
}

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
      char *data_to_send = parse_msg(operacao);
      send(server, data_to_send, strlen(data_to_send), 0);
      do{
        read_v = read(server, buffer, 1024);
      }while(read_v == 0);
      printf("%s\n", buffer);
  }

  return 0;
}
