#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int clientSocket, recvlen;
    char buffer[1024], username[100];
    struct sockaddr_in serverAddr;
    socklen_t addrSize;

    clientSocket = socket(AF_INET, SOCK_DGRAM , 0);
    if(clientSocket < 0)
    {
        perror("Socket Creation Failed\n");
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(2136);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.36");

    printf("Enter your username : ");
    fgets(username, sizeof(username), stdin);
    username[strcspn(username, "\n")] = '\0';
    addrSize = sizeof(serverAddr);

    while(1)
    {
        sleep(1);
        //sendind the username to server which also indicates as a time request
        sendto(clientSocket, username, strlen(username), 0, (struct sockaddr*)&serverAddr, addrSize);
        //receiving time as reply from server
        recvlen = recvfrom(clientSocket, buffer, sizeof(buffer), 0, (struct sockaddr*)&serverAddr, &addrSize);
        if(recvlen > 0)
        {
            buffer[recvlen] = '\0';
            printf("Current Time from Server: %s\n", buffer);
        }
    }

    return 0;
    close(clientSocket);
}
