#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

// struct that contains request packet
// parameters:
//  op[3]: operator
//  op1: first member of operation
//  op2: second member of operation
typedef struct req {
    char op[3];
    int op1;
    int op2;
} req_pkt;

// struct that contains answer packet
// parameters:
//  invalid: flag to tell the client the answer is invalid
//  value: result (if packet is valid) or error code (otherwise)
typedef struct answ {
    int invalid;
    int value;
} answ_pkt;

// function that adds two numbers
// parameters:
//  a: first number
//  b: second number
answ_pkt *add(int a, int b){
    answ_pkt *result = malloc(sizeof(answ_pkt));
    result->invalid = 0;
    result->value = a+b;

    return result;
}

// function that subtract two numbers
// parameters:
//  a: first number
//  b: second number
answ_pkt *sub(int a, int b){
    answ_pkt *result = malloc(sizeof(answ_pkt));
    result->invalid = 0;
    result->value = a-b;

    return result;
}

// function that multiplies two numbers
// parameters:
//  a: first number
//  b: second number
answ_pkt *mul(int a, int b){
    answ_pkt *result = malloc(sizeof(answ_pkt));
    result->invalid = 0;
    result->value = a*b;

    return result;
}

// function that divides two numbers
// parameters:
//  a: first number
//  b: second number
// NOTE: if there is a division by zero, the flag invalid is set to 1
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

// function that receives a request packet,
//  calculates the answer (if the request is valid)
//  and mount the answer packet with proper values
answ_pkt *calc(req_pkt *requisition){
    answ_pkt *answer = malloc(sizeof(answ_pkt));
    if(!(strcmp(requisition->op, "add"))){
        answer = add(requisition->op1, requisition->op2);
    }
    else if(!(strcmp(requisition->op, "sub"))){
        answer = sub(requisition->op1, requisition->op2);
    }
    else if(!(strcmp(requisition->op, "mul"))){
        answer = mul(requisition->op1, requisition->op2);
    }
    else if(!(strcmp(requisition->op, "div"))){
        answer = divi(requisition->op1, requisition->op2);
    }
    else if(!(strcmp(requisition->op, "cls"))){
        answer->invalid = 1000;
    }
    else{
        answer->invalid = 1;
        answer->value = -1;
    }

    return answer;
}

int main(int argc, char* argv[]){
    if(argc < 3){
        printf("Specify port\n");
        exit(1);
    }

    req_pkt *buffer = malloc(sizeof(req_pkt));
    struct sockaddr_in server_address, connection_address;
    int server, client, read_v;
    int addr_length = sizeof(server_address), connection_length = sizeof(connection_address);

    if((server = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        printf("Error on socket\n");
        exit(1);
    }

    // insert family, port and IP to server socket
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(argv[2]));
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    memset(server_address.sin_zero, '\0', sizeof(server_address.sin_zero));

    // bind the socket with the information passed on server_address
    if(bind(server, (struct sockaddr *) &server_address, sizeof(server_address))){
        printf("Error on bind\n");
        exit(1);
    }

    // make socket listen on that port specified on server_address
    if(listen(server, 1) < 0){
        printf("Error on listening\n");
        exit(1);
    }

    printf("Waiting for client to connect...\n");

    // when client connects to the server port, the server accepts the connection
    //  and creates a new socket to be used in the connection
    if((client = accept(server, (struct sockaddr *)&connection_address, (socklen_t*) &connection_length)) < 0){
        printf("Error accepting\n");
        exit(1);
    }

    printf("Client with IP [%s] and port [%d] has connected!\n", inet_ntoa(connection_address.sin_addr),        ntohs(connection_address.sin_port));

    answ_pkt* result = malloc(sizeof(answ_pkt));

    while(1){
        do{
            // read information from client
            read_v = read(client, buffer, sizeof(req_pkt));
        }while(read_v == 0);

        printf("Package received!\n");
        printf("Content: [%s %d %d]\n", buffer->op, buffer->op1, buffer->op2);

        if(strcmp(buffer->op, "cls") == 0){
            close(client);
            exit(0);
        }

        printf("Calculating...\n\n");

        result = calc(buffer);

        // send information to client
        printf("Sending answer to client... ");
        send(client, result, sizeof(answ_pkt), 0);
        printf("Sent!\n\n");
    }

    return 0;
}
