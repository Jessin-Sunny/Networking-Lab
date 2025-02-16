#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main()
{
    int client;
    char buffer[1024];
    struct sockaddr_in servAddr;

    client = socket(AF_INET, SOCK_STREAM, 0);
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(2136);
    servAddr.sin_addr.s_addr = inet_addr("127.0.36");

    if(connect(client,(struct sockaddr *)&servAddr,sizeof(servAddr)) <0)
    {
        perror("Connection Failed");
        return -1;
    }

    printf("Connected to server \n");

    while(1)
    {
        memset(buffer,0,sizeof(buffer));
        printf("Enter a message : ");
        fgets(buffer,sizeof(buffer),stdin);
        buffer[strcspn(buffer,"\n")] = '\0';

        send(client, buffer, strlen(buffer), 0);

        if(strcmp(buffer,"exit") == 0)
        {
            printf("Exiting client...\n");
            break;
        }

        int valread = recv(client, buffer, sizeof(buffer) - 1, 0);
	if(valread > 0)
	{
	    buffer[valread] = '\0';
	    printf("Server: %s\n", buffer);
	}

	if(strcmp(buffer,"exit") == 0)
	{
	    printf("Server exited...Closing client...\n");
	    break;
	}
    }
    close(client);
    return 0;
}
