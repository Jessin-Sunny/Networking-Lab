#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int client;

void *recieve_msg(void *arg)
{
    char buffer[1024];
    int valread;
    while(1)
    {
        memset(buffer, 0, sizeof(buffer));
        valread = recv(client, buffer, sizeof(buffer), 0);
        if(valread <= 0)
        {
            printf("Disconnected\n");
            exit(0);
        }
        printf("%s", buffer);
    }
}

int main()
{
    char username[50], buffer[1024];
    struct sockaddr_in servAddr;


    client = socket(AF_INET, SOCK_STREAM, 0);
    if(client < 0)
    {
        printf("Socket creation failed\n");
        return -1;
    }
    else
    {
        printf("Socket creation successfull\n");
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(3621);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.36");
    socklen_t size = sizeof(servAddr);

    if(connect(client, (struct sockaddr *)&servAddr, size ) < 0)
    {
        printf("Connection failed\n");
        return -1;
    }
     else
    {
        printf("Connection successfull\n");
    }

    printf("Enter your username : ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';

    send(client, username, strlen(username), 0 );

    pthread_t recievethread;
    pthread_create(&recievethread, NULL, recieve_msg, NULL);
    pthread_detach(recievethread);

    printf("Type the Message\n");

    while(1)
    {
        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);
        send(client, buffer, strlen(buffer), 0);
    }

    close(client);
    return 0;

}
