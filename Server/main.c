#include <stdio.h>
#include <string.h>    // for strlen
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h> // for inet_addr
#include <unistd.h>    // for write
#include <time.h>

#define PORT_NUMBER 60000
#define INCORRECT "INCORRECT REQUEST\n"
#define CLOSE "CLOSE_SERVER"
#define DAY "GET_DAY_OF_WEEK"
#define TIME_ZONE "GET_TIME_ZONE"
#define TIME_DATE "GET_TIME_DATE"
#define DATE "GET_DATE"
#define TIME "GET_TIME"
#define BUFF_SIZE 2000

int main(int argc, char *argv[])
{
    int socket_desc , new_socket , c;
    struct sockaddr_in server , client;
    char* request;
    char* response;
    request = calloc(1, BUFF_SIZE);
    response = calloc(1, 100);
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
            
            int requestLength = recv(new_socket, request, BUFF_SIZE, 0);
            if(requestLength < 0){
                return 1;
            }
            
            else if (requestLength > 0)
            {   
                time_t rawtime;
                struct tm* timeinfo;

                
                time(&rawtime);
                timeinfo = localtime(&rawtime);
                request[requestLength - 2] = '\0';
                
                
                if(strcmp(request, CLOSE) == 0){
                    printf("Close signal received.");
                    response = "GOOD BYE\n";
                    send(new_socket, response, strlen(response), 0);
                    close(new_socket);
                    close(socket_desc);
                    sleep(2);
                    return 0;
                }
                else if(strcmp(request, DAY) == 0){
                    strftime(response, sizeof(response), "%A\n", timeinfo);
                    send(new_socket, response, strlen(response), 0);
                }
                else if(strcmp(request, TIME_ZONE) == 0){
                    strftime(response, sizeof(response), "%z", timeinfo);
                    char* toFormat = calloc(1, 2);
                    toFormat[0] = response[3];
                    toFormat[1] = response[4];
                    response[3] = ':';
                    response[4] = toFormat[0];
                    response[5] = toFormat[1];
                    response[6] = '\n';
                    response[7] = '\0';
                    free(toFormat);
                    send(new_socket, response, strlen(response), 0);
                }
                
                else if(strcmp(request, TIME_DATE) == 0){
                    
                    strftime(response, sizeof(response), "%H:%M:%S, %m.%d.%Y\n", timeinfo);
                    response[strlen(response)] = '\n';
                    response[strlen(response) + 1] = '\0';
                    send(new_socket, response, strlen(response), 0);
                    
                }
                /*
                else if(strcmp(request, TIME)){
                    strftime(response, sizeof(response), "%H:%M:%S\n", timeinfo);
                    send(new_socket, response, strlen(response), 0);
                }
                
                
                else if(strcmp(request, DATE)){
                    strftime(response, sizeof(response), "%m/%d/%Y \n", timeinfo);
                    send(new_socket, response, strlen(response), 0);

                }
                */
                else{
                    send(new_socket, INCORRECT, strlen(INCORRECT), 0);
                }
            }
        }
    }
    return 0;
}
