#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

typedef struct
{
    int socket;
    char username[50];
} Client;

Client clients[50];     //maximum of 50 clients
int clients_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;      // intializing clients_mutex to be used for mutex


void broadcast_msg(char *msg, int clientsocket)
{
    pthread_mutex_lock(&clients_mutex);
    for(int i=0; i < clients_count ; i++)
    {
        //sending to every client except sender itself
        if(clients[i].socket != clientsocket)
        {
            send(clients[i].socket, msg, strlen(msg), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void *handle_client(void *arg)
{
    //arg contain infomation about client socket descriptor, got by accept
    char username[50], buffer[1024];
    int clientsocket;
    clientsocket = *(int *)arg;

    //clears username
    memset(username, 0, sizeof(username));

    //recieving username from client
    if(recv(clientsocket, username, sizeof(username), 0) < 0)
    {
        printf("Recieving Failed\n");
        close(clientsocket);
        free(arg);
        return NULL;
    }

    //updating client informations
    pthread_mutex_lock(&clients_mutex);
    clients[clients_count].socket = clientsocket;
    strcpy(clients[clients_count].username, username);
    clients_count++;
    pthread_mutex_unlock(&clients_mutex);

    printf("%s joined chat\n",username);

    //brodcasting joined message to all clients connected
    char joined_msg[100];
    snprintf(joined_msg, sizeof(joined_msg), "%s joined chat\n", username);
    broadcast_msg(joined_msg, clientsocket);

    //reading the message sent by a client and broadcasting it
    while(1)
    {
        int valread;
        memset(buffer, 0, sizeof(buffer));
        valread = recv(clientsocket, buffer, sizeof(buffer), 0);
        if(valread <= 0)
        {
            //valread = 0 means client disconnects
            //valread < 0 means error
            break;
        }
        char format_msg[1024];
        snprintf(format_msg, 2000, "%s : %s\n", username, buffer);
        printf("%s", format_msg);
        broadcast_msg(format_msg, clientsocket);
    }

    //coming out of the while loop indicates, client has disconnected
    //removing disconnected client from the structure
    //then updating the structure
    pthread_mutex_lock(&clients_mutex);
    for(int i = 0; i < clients_count; i++)
    {
        //finding out the removed client
        if(clients[i].socket == clientsocket)
        {
            //shifting the other client to one position left
            for(int j = i; j < clients_count; j++)
            {
                clients[j+1] = clients[j];
            }
            clients_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    //broadcsting left message
    printf("%s left the chat\n", username);
    snprintf(joined_msg, sizeof(joined_msg), "%s left the chat\n", username);
    broadcast_msg(joined_msg, clientsocket);

    close(clientsocket);
    free(arg);
    return NULL;

}

int main()
{

    int server;
    struct sockaddr_in servAddr;
    struct sockaddr_storage store;
    socklen_t size;

    server = socket(AF_INET, SOCK_STREAM, 0);
    if(server < 0)
    {
        printf("Socket Creation Failed\n");
        return -1;
    }
    else
    {
        printf("Socket Creation Successfull\n");
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(3621);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.36");

    if(bind(server, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0)
    {
        printf("Binding Failed\n");
        return -1;
    }

    if(listen(server, 5) == 0)
    {
        printf("Server is Listening\n");
    }

    size = sizeof(servAddr);

    while(1)
    {
        int* newSock = malloc(sizeof(int));
        *newSock = accept(server, (struct sockaddr*)&servAddr, &size);
        if(*newSock < 1)
        {
            printf("Accepting Failed\n");
            free(newSock);
            continue;
        }

        pthread_t client_thread;
        pthread_create(&client_thread, NULL, handle_client, (void *)newSock);
        pthread_detach(client_thread);
    }
    close(server);
    return 0;

}
