#include <stdio.h>
#include <stdlib.h> // atoi
#include <string.h> // memset

#include <sys/types.h> // socket
#include <sys/socket.h> // socket
#include <unistd.h> // close

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

    int bind_v = bind(sock, (struct sockaddr*) &server_addr, sizeof server_addr);

    if(bind_v < 0){
        printf("error on bind\n");
    }

    int listen_v = listen(sock, 5);

    if(listen_v < 0){
        printf("error on listen\n");
    }

    while(1){
        printf("Waiting for a connection...\n");

        struct sockaddr_storage client_addr;

        int client_addr_len = sizeof client_addr;
        int connection_sock = accept(sock, (struct sockaddr*) &client_addr, &client_addr_len);

        if(connection_sock < 0){
            printf("error on accept\n");
        }

        char *message = "Server connected";
        int bytes = send(sock, message, strlen(message), 0);

        if(bytes != strlen(message)){
            printf("Message was not sent completely\n");
        }

        char input[256];

        int recv_bytes = recv(connection_sock, input, 256, 0);

        if(recv_bytes == -1){
            printf("Error on recv_bytes\n");
        }
        else if(recv_bytes == 0){
            printf("client has closed the connection\n");
        }

        if(strcmp(input, "close") == 0){
            printf("Closing connection...\n");
            close(connection_sock);
        }

        printf("Data received: [%s]\n", input);
    }

    return 0;
}
