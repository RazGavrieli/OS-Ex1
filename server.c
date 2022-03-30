// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 3490
int main(int argc, char const *argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
    int connected = 1;

	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) == -1)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
    while (1) {
        if (listen(server_fd, 3) == -1)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        } else {
            printf("listening");
        }
        new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen);
        if (new_socket == -1)
        {
            perror("accept error");
            exit(EXIT_FAILURE);
        }
        printf("cconnected to new connection\n");
        connected = 1;
        while (connected) {
            memset(buffer, 0, 1024);
            valread = read( new_socket , buffer, 1024);  
            if (valread==0) {
                connected = 0;
            } else {
                printf("%s, %d\n",buffer, valread );
            }
        }
    }
    close(new_socket);
	return 0;
}
