#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>     //used for sleep function
#define BUCKET_SIZE 10
#define OUTPUT_RATE 3

void leaky_bucket(int n, int packet[])
{
    int i, bucket, sent;

    bucket = 0;
    printf("Time Step\t|Incoming Packets\t|Bucket Size\t|Sent Packets\t|Remaining\t|\n");
    printf("-----------------------------------------------------------------------------------------\n");

    for(i=0; i<n; i++)
    {
        printf("%d\t\t|%d\t\t\t|", i + 1, packet[i]);

        if(bucket + packet[i] > BUCKET_SIZE)
        {
            printf("Overflow !!!\t|");
            bucket = BUCKET_SIZE;
        }
        else
        {
            bucket += packet[i];
            printf("%d\t\t|",bucket);
        }

        sent = (bucket < OUTPUT_RATE) ? bucket : OUTPUT_RATE;
        bucket -= sent;
        printf("%d\t\t|%d\t\t|\n",sent, bucket);

        sleep(1);
    }

    //After all incoming packets are received
    //Senting Remaining Packets from bucket
    while(bucket > 0)
    {
        sent = (bucket < OUTPUT_RATE) ? bucket : OUTPUT_RATE;
        printf("Drain\t\t|---\t\t\t|%d\t\t|%d\t\t|%d\t\t|\n", bucket, sent, bucket - sent);
        bucket = bucket - sent;
        sleep(1);
    }
}

int main()
{
    int i, n, packet[25];

    printf("How many time steps ? : ");
    scanf("%d", &n);
    printf("Enter the total number incoming packets at each step\n");
    for(i=0; i<n; i++)
    {
        scanf(" %d", &packet[i]);
    }

    leaky_bucket(n, packet);

    return 1;
}

/*
Regarding Formatting a table like output --- Tips
-> Use two tab spaces for almost all output datas
-> Use a single tab space for Overflow!!! & Table Headings
-> Use three tab spaces for data corresponding to incoming packets field
*/
