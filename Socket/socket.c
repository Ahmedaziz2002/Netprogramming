#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

int main(){
    int sockfd=socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        perror("Socket creatio failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successfully\n");
    close(sockfd);
    return 0;
}