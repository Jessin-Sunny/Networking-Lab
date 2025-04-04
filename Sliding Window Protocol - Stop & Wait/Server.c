#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#define MSG_LEN 5
#define TIME 2          //defining timeout in seconds

int main()
{
    int server, newSock, ack, seq;
    struct sockaddr_in servAddr;
    struct sockaddr_storage store;
    socklen_t addrSize;
    struct timeval timeout = {TIME, 0};


    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0)
    {
        perror("Socket failed");
        return -1;
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(6265);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(server, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        perror("Bind failed");
        return -1;
    }

    if (listen(server, 5) == 0)
    {
        printf("Server is listening...\n");
    }
    else
    {
        perror("Listen failed");
        return -1;
    }

    //accepting a client connection
    newSock = accept(server, (struct sockaddr *)&store, &addrSize);
    if (newSock < 0)
    {
        printf("Accept failed\n");
    }
    else
    {
        printf("Client Connected....\n");
    }

    //receiving message from client
    //Assuming the total number of messages equals the MSG_LEN
    //setting timeout using setsockopt
    //Acknowledgement means which sequence number packet is needed for server from client
    setsockopt(newSock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    ack = 0;
    while(ack < MSG_LEN)
    {
        if(recv(newSock, &seq, sizeof(seq), 0) > 0)
        {
            if(seq == ack)
            {
                printf("Received Packet with Sequence Number: %d\n", seq);
                ack = ack +1;
                send(newSock, &ack, sizeof(ack), 0);
                printf("Sent Acknowlegement: %d\n", ack);
            }
        }
        else
        {
            printf("TimeOut !!! Error in Receiving\n");
        }
    }

    close(newSock);
    close(server);
    return 0;

}
