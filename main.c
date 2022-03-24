#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <unistd.h> // getcwd

#include <sys/socket.h> // tcp magic
#include <arpa/inet.h>  // tcp magic

#include <dirent.h> //opendir\readdir\closedir..

#include <stdlib.h> // for system

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

void echo(char** output, size_t outputsize, bool local, int sock)
{
    getline(output, &outputsize, stdin);
    if (local) {  
        printf("%s", *output);
    } else {    
        send(sock , *output , strlen(*output) , 0 );
    }
}

void change_dir(char *cwd) 
{
    /**
     * @brief This function uses a system call (chdir()) that uses the kernal inorder to navigate through the directory. 
     * 
     */
    char dirname[256];
    scanf("%s", dirname);
    if (chdir(dirname) == -1) {
        printf("couldnt open\n");
    } else {
        getcwd(cwd, sizeof(cwd));
    }
}

void read_dir(char *cwd) {
    DIR* dir = opendir(cwd); // might change cwd to "."
    if (dir == NULL) {
        printf ("Cannot open directory '%s'\n", cwd);
        return;
    }

    struct dirent* entity;
    entity = readdir(dir);
    while (entity != NULL)
    {
    printf("%s\n",entity->d_name);
    entity = readdir(dir);
    }
    closedir(dir);
}

int copy_file() {
    /**
     * @brief This function is used to copy a file from the current folder to the destination folder.
     * Keep in mind that the file HAS to be in the current folder. And the destination folder has to be in the current folder as well. (following the assigemnt's instructions)
     * I used (chdir) which is a system call, so my implementation uses a system call. Notice that fopen\read\write are library functions. 
     */
    char filenameSRC[256];
    char destinationfolderDST[256];
    scanf("%s", filenameSRC);
    scanf("%s", destinationfolderDST);
    if (access(filenameSRC, 0)==0&&access(destinationfolderDST, 0)==0) {
        FILE* file2copy = fopen(filenameSRC, "rb");
        chdir(destinationfolderDST);
        FILE* file2create = fopen(filenameSRC, "wb");
        if (file2copy==NULL||file2create==NULL) {
            return -1;
        }
        while (1) {
            int c = fgetc(file2copy);
            if (feof(file2copy)) {
                break;
            }
            fwrite(&c, sizeof(c), 1, file2create);
        }
        fclose(file2copy);
        fclose(file2create);
        chdir("..");
        return 1;
    } else {
        return -1;
    }
}

int delete_file(char* cwd) {
    /**
     * @brief 
     * My implementation uses unlink() which in a unix-based OS is a system-call.
     */
    char filename[256];
    scanf("%s", filename);
    return unlink(filename);
}
int main ()
{
    //size for input
    size_t t = 256;
    char input[t];

    size_t outputsize = 128;
    char **output; 

    bool running = true;
    bool local = true; // changes to false when TCP connection has been established

    char cwd[256];

    int sock;

    
    while (running)
    {
        getcwd(cwd, sizeof(cwd));
        printf("%s yes master\n", cwd);
        scanf("%s", input);

        if (!strcmp(input, "EXIT")) {
            running = false;
        } else if (!strcmp(input, "ECHO")) {
            output = (char **)malloc((sizeof(char)*outputsize));
            echo(output, outputsize, local, sock);
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
        } else if (!strcmp(input, "DIR")) {     
            read_dir(cwd);
        } else if (!strcmp(input, "CD")) {
            change_dir(cwd);
        } else if (!strcmp(input, "COPY")) {
            if (copy_file()==1) {
                printf("file copied\n");
            } else {
                printf("ERROR in copying the file, make sure input is correct. use:\nCOPY (NAME OF THE FILE) (DESTINATION FOLDER)\n");
            }
        } else if (!strcmp(input, "DELETE")) {
            if (delete_file(cwd)==0) {
                printf("file deleted\n");
            } else {
                printf("ERROR in deleteing the file, make sure input is correct.\n");
            }   
        } else {
            printf("unkown command, uses system..\n");
            char ** output_malloc = (char **)malloc((sizeof(char)*outputsize));
            getline(output_malloc, &outputsize, stdin);//segmentation fault
            char* cmd = strcat(input,*output_malloc);
            printf("%s", cmd);
            system(cmd);
            /**
             * System is a library function, laies in the standard library, that takes an argument and uses it 
             * in order to call commands usually used in the terminal.
             */
            int pid = fork();
            if (pid==0) {
                char *cmd = "ls";
                char *argv[3];
                argv[0] = "ls";
                argv[1] = "-l";
                argv[2] = NULL;
                if (execvp(cmd, argv) == -1) {
                    printf("fork failed");
                }
                
                break;
        }
        }

    }
    


    return 1;
}