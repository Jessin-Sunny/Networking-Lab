#include <stdio.h>
#include <limits.h>

void main()
{
    int n, source, i, j, adj[50][50], d;
    int dist[50], visited[50], round, v;
    int min, min_index;
    printf("How many nodes ? : ");
    scanf("%d", &n);

    printf("Enter the adjacency matrix\n");
    for(i=0; i<n; i++)
    {
        for(j=0; j<n; j++)
        {
            scanf("%d", &adj[i][j]);
        }
    }

    //set all distance to INT_MAX and visited to 0
    for(i=0; i<n; i++)
    {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }

    printf("Enter the source node: ");
    scanf("%d", &source);  // User selects the source node
    dist[source] = 0;   //distance from source to source is 0
    //n-1 times running checking for other n-1 nodes
    for(round = 0; round < n - 1; round++)
    {
        min = INT_MAX;
        //to find the unvisited node with the smallest distance 
        for(v = 0; v<n; v++)
        {
            //finding link with minimum distance
            if(visited[v] == 0 && dist[v] < min)
            {
                min = dist[v];
                min_index = v;
            }
        }
        visited[min_index] = 1;
        //iterates over all nodes to check if the current node has a direct link to another node
        for(d=0; d<n; d++)
        {
            //d is not visited and There exists a direct link and he current shortest distance to min_index is not infinity and The new possible distance is less than the current stored distance
            if(!visited[d] && adj[min_index][d] && dist[min_index] != INT_MAX && dist[min_index] + adj[min_index][d] < dist[d])
            {
                dist[d] = dist[min_index] + adj[min_index][d];
            }
        }
    }
    printf("\nVertex distance from source\n");
    for(i=0; i<n; i++)
    {
        if (dist[i] == INT_MAX)
        {
            printf("%d\t\tINF\n", i);
        }
        else
        {
            printf("%d\t\t%d\n",i,dist[i]);
        }
    }
}
