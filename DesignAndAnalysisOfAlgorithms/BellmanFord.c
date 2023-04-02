#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

struct graph	// structure for graph
{
	int source;
	int neighbour;
	int edge_weight;
	struct graph *head;	// for array's branch
	struct graph *next;	// for branch element
};
typedef struct graph* graphptr;

struct ver	// structure for vertices
{
	int num;
};
typedef struct ver* vertex;

struct node	// structure for stack for negative cycle
{
	int num;
	struct node *next;
};
typedef struct node * nodeptr;

int push(nodeptr *insert,nodeptr *curptr,int ele)	// stack push
{
	*curptr=*insert;
	*insert=(nodeptr) malloc(sizeof(struct node));
	if (*insert!=NULL)
	{
		//scanf("%d",&((*insert)->num));
		(*insert)->num=ele;
		(*insert)->next=(*curptr);
	}
	else
	{
		printf("Push Failure\n");
		exit(1);
	}
}
int display(nodeptr *insert,nodeptr *head)	// stack display
{
	if (*insert==NULL)
	{
		printf("Empty List\n");
	}
	else
	{
		// loop
		*head=*insert;
		while (*insert!=NULL)
		{
			printf("%d ",(*insert)->num);
			*insert=(*insert)->next;
		}
		*insert=*head;
	}
}

int graph_branch(graphptr *front,graphptr *back,graphptr *temp,int a,int b,int e_weight)	// making queue for edge and edge_weight
{
	
	*temp=*back;
	*back=(graphptr) malloc(sizeof(struct graph));
	if (*back==NULL)
	{
		return(0);
	}
	else
	{
		(*back)->source=a;				// storing source
		(*back)->neighbour=b;			// storing it's neighbour
		(*back)->edge_weight=e_weight;	// storing it's edge weight
		(*back)->next=NULL;
		if ((*front)==NULL)
		{
			*front=*back;
		}
		else
		{
			(*temp)->next=(*back);
		}
		return(1);
	}
}

void ShortestPath(graphptr front,graphptr temp,int n,int m)	// BellmanFord algorithm
{
	// structure difined for vertices
	vertex pred[n+1];
	int size=n;
	
	for (int i=1 ; i<=n ; i++)
	{
		pred[i]=(vertex) malloc(sizeof(struct ver));
		((pred[i])->num)=-1;
	}
	int temp_arr[n+1];		// has dist value corresponding key
	temp_arr[m]=0;			// source dist 0
	int keys=0;
	for (int i=2 ; i<=n ; i++)
	{
		keys+=1;
		if (keys==m){
			keys+=1;
		}
		temp_arr[keys]=2000000000;
		//temp_key_ind[keys]=i;
	}
	//printf("hello1\n");
	int flag=0,elementf=0,elementl=0,p=0;
	for (int i=1; i<=n; i++){						// n times loop for checking negative edge cycle
		flag=0;
			graphptr loop=front;
			while (loop!=NULL)						// going through loop (queue) of edges and it's weight
			{
				if (temp_arr[loop->source] + loop->edge_weight < temp_arr[loop->neighbour]){
					int x=temp_arr[loop->source] + loop->edge_weight;
					pred[loop->neighbour]->num=loop->source;
					temp_arr[loop->neighbour]=x;
					flag=1;
					if (i==n && p==0){
						elementf=loop->neighbour;	// most recent changed number
						p=1;
					}
					elementl=loop->neighbour;		// negative cycle changed number
				}
				loop=loop->next;
			}
		if (flag==0){
			break;
		}
	}
	if (flag==1){								// if negative weight cycle found Flag=1
		nodeptr head=NULL;
		nodeptr curptr=NULL;
		nodeptr insert=NULL;
		//printf("Negative edge cycle\n");
		int check=0,check_ele=pred[elementf]->num;	// checking whether elementf (first) is element of negative weight cycle
		for (int i=1; i<=n; i++){
			if (check_ele==elementf){
				check=1;
				break;
			}
			check_ele=pred[check_ele]->num;
		}
		if (check==1){								// if it is then go through it's predecessors put in stack and print
			int nc_ele=pred[elementf]->num;	//negative cycle element
			push(&insert,&curptr,elementf);
			while (nc_ele!=elementf){
				//printf("%d ",nc_ele);
				push(&insert,&curptr,nc_ele);
				nc_ele=pred[nc_ele]->num;
			}
		}else{										// else go through elementl (last) which is negative cycle element 
			int nc_ele=pred[elementl]->num;	//negative cycle element
			push(&insert,&curptr,elementl);
			while (nc_ele!=elementl){
				//printf("%d ",nc_ele);
				push(&insert,&curptr,nc_ele);
				nc_ele=pred[nc_ele]->num;
			}
		}
		display(&insert,&head);						// finally display the stack
	}else{
		for (int i=1; i<=size; i++){
			if (temp_arr[i]==2000000000){
				printf("Unreachable\n");	
			}else{
				printf("%d %d\n",temp_arr[i],pred[i]->num);	// if not negative weight cycle then print shortest distance and it's predecessor
			}
		}
	}
	
}

int main()
{
	int n=0,m=0,a=0,b=0,e_weight=0,edges=0;
	scanf("%d%d",&n,&edges);

	graphptr front=NULL;
	graphptr back=NULL;
	graphptr temp=NULL;
	for (int i=1 ; i<=edges ; i++)
	{
		scanf("%d%d%d",&a,&b,&e_weight);
		graph_branch(&front,&back,&temp,a,b,e_weight);
	}

	// int dag=0;
	int start_num=0;		// number for starting dfs
	scanf("%d",&start_num);
	// calling depth first search
	ShortestPath(front,temp,n,start_num);
}
