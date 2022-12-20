#include<stdio.h>
#include<string.h>    // for strlen
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h> // for inet_addr
#include<unistd.h>    // for write
 #include <time.h>

#define PORT_NUMBER 60000
#define INCORRECT "INCORRECT REQUEST\n"
#define CLOSE "CLOSE_SERVER"
#define DAY "GET_DAY_OF_WEEK"

int main(int argc, char *argv[])
{
    int socket_desc , new_socket , c;
    struct sockaddr_in server , client;
    char* request = calloc(1, 2000 * sizeof(char));
    char* response;
     
    // Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1)
    {
        puts("Could not create socket");
        return 1;
    }
     
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT_NUMBER);
    
    // Bind
    if(bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        puts("Bind failed");
        return 1;
    }
    puts("Socket is binded");
     
    // Listen
    listen(socket_desc, 3);
     
    // Accept and incoming connection
    puts("Waiting for incoming connections...");
    
   
    c = sizeof(struct sockaddr_in);
    while( new_socket =
           accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c) )
    {
        puts("Connected.");
        while (1)
        {
            int messageLength = recv(new_socket, request, 2000, 0);
            if(messageLength < 0){
                return 1;
            }
            else if (messageLength > 0)
            {   
                request[messageLength - 2] = '\0';

                /*
                if(messageLength == strlen(CLOSE) + 2 && strncmp(request, CLOSE, strlen(CLOSE)) == 0 ) {
                    close(socket_desc);
                    return 0;
                }
                else if(messageLength == strlen(DAY) + 2 && strncmp(request, DAY, strlen(DAY)) == 0 ) {
                    response = "gün";
                    send(new_socket, response, strlen(response), 0);
                    close(socket_desc);
                }
                */
                if(strcmp(request, CLOSE) == 0){
                    close(new_socket);
                    close(socket_desc);
                    return 0;
                }
                else if(strcmp(request, DAY) == 0){
                    response = system("date");
                    send(new_socket, response, strlen(response), 0);
                }
                else{
                    
                    send(new_socket, INCORRECT, strlen(INCORRECT), 0);
                }
            }
        }
        
        
        
    }
    close(socket_desc);
    return 0;
}
