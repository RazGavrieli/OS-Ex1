#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <unistd.h> // getcwd

#include <sys/socket.h> // tcp magic
#include <arpa/inet.h>  // tcp magic

int tcp_connection(int port) 
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
       
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    return sock;
}

int main ()
{
    //size for input
    size_t t = 256;
    char input[t];

    size_t outputsize = 128;
    char *output[outputsize]; 

    bool running = true;
    bool local = true; // changes to false when TCP connection has been established

    char cwd[256];

    int sock;

    while (running)
    {
        getcwd(cwd, sizeof(cwd));
        printf("%s yes master\n", cwd);
        scanf("%s", input);
        //getline(input, &t, stdin);
        //printf("\t%s\n", input);
        if (!strcmp(input, "EXIT")) {
            running = false;
        } else if (!strcmp(input, "ECHO")) {
            getline(output, &outputsize, stdin);
            if (local) {  
                printf("%s", *output);
            } else {    
                send(sock , *output , strlen(*output) , 0 );
            }
        } else if (!strcmp(input, "TCP")) {
            int port = 0;
            scanf("%d", &port);
            sock = tcp_connection(port);
            if (sock == -1) {
                printf("ERROR IN TCP CONNECTION\n");
            } else {
                local = false;
            }
        } else if (!strcmp(input, "LOCAL")) {
            local = true;
            close(sock);
        }
        

    }
    


    return 1;
}