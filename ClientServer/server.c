#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<sys/select.h>
#include<arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

int main(){
    int server_fd,new_socket,client_sockets[MAX_CLIENTS],max_sd,sd,activity;
    struct sockaddr_in address;
    fd_set readfds;
    int addrlen=sizeof(address);
    char buffer[BUFFER_SIZE]={0};

    for(int i=0;i<MAX_CLIENTS;i++){
        client_sockets[i]=0;
    }

    //STEP 1:CREATE A SOCKET
    server_fd=socket(AF_INET,SOCK_STREAM,0);
    if(server_fd==0){
        perror("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }

    //STEP 2:BIND TO IP AND PORT
    address.sin_family=AF_INET;
    address.sin_addr.s_addr=INADDR_ANY;
    address.sin_port=htons(PORT);

    if(bind(server_fd,(struct sockaddr*)&address,sizeof(address))<0){
        perror("Bind failed\n");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    //STEP 3:LISTEN FOR CONNECTIONS
    if(listen(server_fd,3)<0){
        perror("Listen failed\n");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n",PORT);

    while(1){
        //clear the socket and add the server socket
        FD_ZERO(&readfds);
        FD_SET(server_fd,&readfds);
        max_sd=server_fd;

        for(int i=0;i<MAX_CLIENTS;i++){
            sd=client_sockets[i];
            if(sd>0){
                FD_SET(sd,&readfds);
            }
            if(sd>max_sd){
                max_sd=sd;
            }
        }

        //Wait for activity on one of the sockets
        activity=select(max_sd+1,&readfds,NULL,NULL,NULL);
        if(activity<0){
            perror("Select error\n");
        }
        //Check for new connections
        if(FD_ISSET(server_fd,&readfds)){
            new_socket=accept(server_fd,(struct sockaddr*)&address,(socklen_t*)&addrlen);
            if(new_socket<0){
                perror("Accept error\n");
                continue;
            }
            printf("New connection established\n");

            //Add new socket to client socket arrays
            for(int i=0;i<MAX_CLIENTS;i++){
                if(client_sockets[i]==0){
                    client_sockets[i]=new_socket;
                    break;
                }
            }
        }
        //Handle I/O for each client
        for(int i=0;i<MAX_CLIENTS;i++){
            sd=client_sockets[i];
            if(FD_ISSET(sd,&readfds)){
                memset(buffer,0,BUFFER_SIZE);
                int bytes_read=read(sd,buffer,BUFFER_SIZE);

                if(bytes_read==0){
                    //client disconnected
                    printf("Client disconnected\n");
                    close(sd);
                    client_sockets[i]=0;
                }else{
                    //echo message back to client
                    printf("Client :%s\n",buffer);
                    send(sd,buffer,strlen(buffer),0);
                }
            }
        }
    }
    close(server_fd);
    return 0;
}