#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void sendFile(int client)
{
    char buffer[4098], filename[128];
    int bytesRead = 0;
    FILE *fp;
    printf("Enter the Filename : ");
    scanf("%s", filename);
    fp = fopen(filename, "r");
    if(fp == NULL)
    {
        printf("Error in Opening File\n");
    }
    else
    {
        while((bytesRead = fread(buffer, sizeof(char), sizeof(buffer), fp)) > 0)
        {
            send(client, buffer, strlen(buffer), 0);
        }
        printf("%s Sent Successfully...\n", filename);
        fclose(fp);
    }
}

int main()
{
    int client;
    struct sockaddr_in servAddr;
    socklen_t addrSize;

    client = socket(AF_INET, SOCK_STREAM, 0);
    if(client < 0)
    {
        printf("Socket Creation Failed..\n");
        return -1;
    }
    else
    {
        printf("Socket Created Successfully...\n");
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(3621);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.36");
    addrSize = sizeof(servAddr);

    if(connect(client, (struct sockaddr*)&servAddr, addrSize) < 0)
    {
        printf("Connection Failed...\n");
        return -1;
    }
    else
    {
        printf("Connected to Server...\n");
    }

    sendFile(client);

    close(client);
    return 0;
}
