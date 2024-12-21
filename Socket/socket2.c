#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>

int main(){
    int heyud=socket(AF_INET,SOCK_DGRAM,0);
    if(heyud<0){
        perror("Socket creation failed\n");
        exit(EXIT_FAILURE);
    }
    printf("UDP socket created\n");
    close(heyud);
    return 0;
}