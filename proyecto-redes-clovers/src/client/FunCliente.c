#include "FunCliente.h"

int init_cliente(char* IP, int port){
    int client_Socket = socket(PF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_aton(IP, &server_addr.sin_addr);
    server_addr.sin_port = htons(port);

    socklen_t addr_size = sizeof server_addr;
    connect(client_Socket, (struct sockaddr *) &server_addr, addr_size);
	printf("Connectado al servidor!\n");

	return client_Socket;
}