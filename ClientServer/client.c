#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main(){
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE]={0};
    char user_input[BUFFER_SIZE];

    //CREATE A SOCKET
    sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock==0){
        perror("Socket not created\n");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(PORT);

    //convert ipv4 address to binary
    if(inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr)<=0){
        perror("Invalid address\n");
        exit(EXIT_FAILURE);
    }

    //CONNECT WITH SERVER
    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0){
        perror("Connection failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Connected to the server\n");
    while(1){
        printf("Enter your message.Press q to quit\n");
        fgets(user_input,BUFFER_SIZE,stdin);
        user_input[strcspn(user_input,"\n")]=0;

        if(strcmp(user_input,"q")==0){
            printf("Exiting...\n");
            break;
        }

        send(sock,user_input,strlen(user_input),0);

        memset(buffer,0,BUFFER_SIZE);
        int valread=read(sock,buffer,BUFFER_SIZE);
        if(valread>0){
            printf("Server :%s\n",buffer);
        }
    }
    close(sock);

    return 0;
}