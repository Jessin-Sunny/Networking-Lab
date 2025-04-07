#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

typedef struct
{
    int socket;
    char username[100];
    struct sockaddr_in clientAddr;
    socklen_t addrSize;
} Client;

void* handle_client(void* arg)
{
    Client* clientData = (Client *)arg;
    char timeData[128];

    time_t current = time(NULL);        //return current time but not in human readable format
    struct tm *currentinfo = localtime(&current);       //convert to predefined structure format
    strftime(timeData, sizeof(timeData), "%d-%m-%Y %H:%M:%S", currentinfo);     //converts to human readable

    sendto(clientData->socket, timeData, strlen(timeData), 0, (struct sockaddr*)&(clientData->clientAddr), clientData->addrSize);
    printf("%s:\t%s\n", clientData->username, timeData);

    free(clientData);
    pthread_exit(NULL);

}


int main()
{

    int serverSocket, recvlen;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize;
    char username[100];

    serverSocket = socket(AF_INET, SOCK_DGRAM , 0);
    if(serverSocket < 0)
    {
        perror("Socket Creation Failed\n");
        return -1;
    }
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(2136);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.36");

    if(bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
    {
        perror("Binding Failed\n");
        close(serverSocket);
        return -1;
    }

    printf("Server is listening...\n");
    addrSize = sizeof(clientAddr);
    printf("Time Request Information\n");

    while(1)
    {
        memset(username, 0, sizeof(username));
        recvlen = recvfrom(serverSocket, username, sizeof(username), 0, (struct sockaddr*)&clientAddr, &addrSize);
        if(recvlen > 0)
        {
            //creating a structure that contain client information for incoming client request
            //unlike TCP, here each client information will not be stored like an array
            //Each thread contains information to that client
            //UDP is connection less so we cannot establish direct connection
            //So data has to be sent to the client address i.e. clientAddr
            Client* clientData = malloc(sizeof(Client));
            if (!clientData)
            {
                perror("malloc failed");
                continue;
            }
            strcpy(clientData->username, username);
            clientData->socket = serverSocket;
            clientData->clientAddr = clientAddr;
            clientData->addrSize = addrSize;

           //Creating a thread for client and passing information stored from structure to it
           pthread_t client_thread;
           pthread_create(&client_thread, NULL, handle_client, clientData);
           pthread_detach(client_thread);
        }
    }

    close(serverSocket);
    return 0;
}
