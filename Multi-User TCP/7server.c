#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/select.h>

#define MAX_CLIENTS 20

int main()
{
    int server,newSock;
    char buffer[1024];
    struct sockaddr_in servAddr, clientAddr;
    socklen_t addrSize;
    int clientSockets[MAX_CLIENTS] = {0};
    fd_set readfds;

    server = socket(AF_INET, SOCK_STREAM, 0);
    if(server < 0)
    {
	perror("Socket Failed");
	return -1;
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(2136);
    servAddr.sin_addr.s_addr = inet_addr("127.0.36");

    if(bind(server,(struct sockaddr *)&servAddr,sizeof(servAddr)) <0)
    {
	perror("Bind Failed");
	return -1;
    }

    if(listen(server,5) == 0)
    {
	printf("Server is listening...\n");
    }
    else
    {
	perror("Listening Failed");
	return -1;
    }

    while(1)
    {
	FD_ZERO(&readfds);
	FD_SET(server, &readfds);
	int max_sd = server;

        for(int i=0;i< MAX_CLIENTS; i++)
	{
	    if(clientSockets[i] > 0)
	    {
		FD_SET(clientSockets[i], &readfds);
	    }
	    if(clientSockets[i] > max_sd)
	    {
		max_sd = clientSockets[i];
            }
	}
	int activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
	if(activity < 0)
	{
	    perror("Error in Selecting Clients");
	    continue;
        }
        if(FD_ISSET(server, &readfds))
	{
	    addrSize = sizeof(clientAddr);
	    newSock = accept(server, (struct sockaddr*)&clientAddr, &addrSize);
	    if(newSock < 0)
	    {
		perror("Accept Failed");
		continue;
	    }

	    printf("New Client Connected (Socket : %d)\n", newSock);

            for (int i=0;i < MAX_CLIENTS;i++)
	    {
		if(clientSockets[i] == 0)
		{
		    clientSockets[i] = newSock;
		    break;
		}
            }
	}
	for(int i=0;i<MAX_CLIENTS;i++)
	{
	    int sd = clientSockets[i];

	    if(FD_ISSET(sd, &readfds))
	    {
		memset(buffer, 0, sizeof(buffer));
		int valread = recv(sd, buffer, sizeof(buffer) - 1, 0);

		if(valread == 0)
		{
		    printf("Client on socket %d disconnected.\n", sd);
		    close(sd);
		    clientSockets[i] = 0;
		}
		else
		{
		    buffer[valread] = '\0';
		    printf("Client %d: %s\n", sd, buffer);
		}
		if(strcmp(buffer, "exit") == 0)
		{
		    printf("Closing connection with client : %d\n", sd);
		    close(sd);
		    clientSockets[i]=0;
		}
		else
		{
		    memset(buffer, 0, sizeof(buffer));
		    printf("Enter message for client %d : ",sd);
		    fgets(buffer, sizeof(buffer), stdin);
                    buffer[strcspn(buffer, "\n")] = '\0';

		    send(sd, buffer, strlen(buffer), 0);

		    if(strcmp(buffer, "exit") == 0)
		    {
			printf("Closing Server \n");
			close(server);
			return 0;
		    }
		}
            }
	}
    }
    close(server);
    return 0;
}

