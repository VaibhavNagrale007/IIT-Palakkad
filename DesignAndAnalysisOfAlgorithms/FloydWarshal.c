#include<stdio.h>
int V;
// printing matrix
void print_ans(int dist[][V],int pred[][V],int V)
{
    for (int i = 1; i <= V; i++)
    {
        for (int j = 1; j <= V; j++)
        {
            if (j!=V){
            	if (dist[i][j] == 55555)
		            printf("%d ",55555);
		        else
		            printf("%d ", dist[i][j]);
            }else{
            	if (dist[i][j] == 55555)
		            printf("%d",55555);
		        else
		            printf("%d", dist[i][j]);
            }
        }
        printf("\n");
    }
}

void print_pred(int dist[][V],int pred[][V],int V)
{
    for (int i = 1; i <= V; i++)
    {
        for (int j = 1; j <= V; j++)
        {
            if (j!=V){
            	if (pred[i][j] == -1)
		            printf("NIL ");
		        else
		            printf("%d ", pred[i][j]);
            }else{
            	if (pred[i][j] == -1)
		            printf("NIL");
		        else
		            printf("%d", pred[i][j]);
            }
        }
        printf("\n");
    }
}

void FloydWarshall (int graph[][V],int pred[][V],int V)
{
    int dist[V][V];
 	// copying to dist matrix
    for (int i = 1; i <= V; i++)
    	for (int j = 1; j <= V; j++)
        	dist[i][j] = graph[i][j];
     
    for (int k = 1; k <= V; k++)
    {
        for (int i = 1; i <= V; i++)
        {
            for (int j = 1; j <= V; j++)
            {
                if (dist[i][j] > dist[i][k] + dist[k][j] )
		        {
		        	dist[i][j] = dist[i][k] + dist[k][j];
		         	pred[i][j]=pred[k][j];
		        }
            }
        }
        // condition for printing
        // during 2nd iteration and the last iteration
        if (k==2 || k==V){
        	if (k==2){
        		printf("Pairwise Distance Matrix After Two Rounds\n");
        	}else{
        		printf("Final Pairwise Distance Matrix\n");
        	}
        	print_ans(dist,pred,V);
        	if (k==2){
        		printf("Pairwise Predecessor Matrix After Two Rounds\n");
        	}else{
        		printf("Final Pairwise Predecessor Matrix\n");
        	}
        	print_pred(dist,pred,V);
        }
    }
}

int main()
{ 
	// int V=0;
	scanf("%d",&V);
    int graph[V][V],pred[V][V];
    
    for (int i=1; i<=V; i++){
    	for (int j=1; j<=V; j++){
    		scanf("%d",&graph[i][j]);
    		if (graph[i][j]==55555 || graph[i][j]==0){
    			pred[i][j]=-1;
    		}else{
    			pred[i][j]=i;
    		}
    	}
    }
    
    // calling FloydWarshall function
    FloydWarshall(graph,pred,V);
    return 0;
}