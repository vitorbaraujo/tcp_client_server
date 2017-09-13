#include <stdio.h>
#include <stdlib.h> // atoi
#include <string.h> // memset

#include <sys/types.h>
#include <sys/socket.h>

#include <arpa/inet.h> // htons

int main(int argc, char * argv[]){
    if(argc < 2){
        printf("You need to specify the port\n");
    }

    int port = atoi(argv[1]);

    // create tcp socket
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock < 0){
        printf("error opening socket\n");
    }

    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    memset(server_addr.sin_zero, '\0', sizeof server_addr.sin_zero);

    // NOTE check server_addr.sin_zero

    int bind_v = bind(sock, (struct sockaddr*) &server_addr, sizeof server_addr);

    if(bind_v < 0){
        printf("error on bind\n");
    }

    listen(sock, 5);

    struct sockaddr_storage client_addr;

    int client_addr_len = sizeof client_addr;
    int connection_sock = accept(sock, (struct sockaddr*) &client_addr, &client_addr_len);

    char *message = "Server connected";
    int bytes = send(sock, message, strlen(message), 0);

    if(bytes != strlen(message)){
        printf("Message was not sent completely\n");
    }

    char input[256];
    
    int recv_bytes = recv(connection_sock, input, 1024, 0);

    if(recv_bytes == -1){
        printf("Error on recv_bytes\n");
    }
    
    // NOTE treat received data

    return 0;
}
