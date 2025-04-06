#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void receiveFile(int newSock)
{
    //Function receives and reverses file contents
    char buffer[4098];
    int bytesRead = 0;
    int i,j;
    char temp;
    FILE *fp;
    fp = fopen("output.txt", "w");
    if(fp == NULL)
    {
        printf("Error in Opening File\n");
    }
    else
    {
        while(bytesRead = recv(newSock, buffer, sizeof(buffer), 0) > 0)
        {}
        buffer[strlen(buffer)-1]='\0';
        printf("File Received Successfully...\n");
        //Reversing File Contents
        i = 0;j = strlen(buffer) - 1;
        while(i < j)
        {
            temp = buffer[j];
            buffer[j] = buffer[i];
            buffer[i] = temp;
            i++;j--;
        }
        fprintf(fp, "%s\n", buffer);
        printf("Reversed File Contents\n");
        printf("%s\n", buffer);
        fclose(fp);
    }
}

int main()
{
    struct sockaddr_in servAddr;
    struct sockaddr_storage store;
    socklen_t addrSize;
    int server, newSock;

    server = socket(AF_INET, SOCK_STREAM, 0);
    if(server < 0)
    {
        printf("Socket Creation Failed...\n");
        return -1;
    }
    else
    {
        printf("Socket Created Successfully...\n");
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(3621);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.36");

   if(bind(server, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
   {
        printf("Bindind Failed....\n");
        return -1;
   }
   else
   {
        printf("Binding Successfull....\n");
   }

   if(listen(server, 5) == 0)
   {
        printf("Server is Listening...\n");
   }
   else
   {
        printf("Failed to Listen..\n");
        return -1;
   }

   newSock = accept(server, (struct sockaddr *)&store, &addrSize);
   if(newSock > 0)
   {
        printf("Client Connected....\n");
        receiveFile(newSock);
   }
   close(newSock);
   return 0;
}
