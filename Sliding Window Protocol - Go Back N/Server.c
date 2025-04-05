#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#define TOTAL_PACKETS 10
#define TIME 2          //defining timeout in seconds

int main()
{
    int server, newSock, ack, seq, exp_seq;
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
    //Assuming the total number of messages equals the TOTAL_PACKETS
    //setting timeout using setsockopt
    //Acknowledgement means which sequence number packet is needed for server from client
    setsockopt(newSock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    exp_seq = 0;        //expected sequence number as ordering is important in Go Back N
    ack = 0;
    while(exp_seq < TOTAL_PACKETS)
    {
        if(recv(newSock, &seq, sizeof(seq), 0) > 0)
        {
            printf("Received Packet with sequence number %d\n", seq);
            if(exp_seq == seq)
            {
                exp_seq++;
            }
            ack = exp_seq;
            send(newSock, &ack, sizeof(ack), 0);
            printf("Sent Acknowledgement: %d\n", ack);
        }
        else
        {
            printf("Timeout!!!\n");
        }
    }

    close(newSock);
    close(server);
    return 0;
}
