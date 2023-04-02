#include<stdio.h>
#include<stdlib.h>

struct graph	// structure for graph
{
	int num;
	struct graph *head;	// for array's branch
	struct graph *next;	// for branch element
};
typedef struct graph* graphptr;

struct ver	// structure for vertices
{
	int num;
};
typedef struct ver* vertex;

struct node		// structure for stack
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
		(*insert)->num=ele;
		(*insert)->next=(*curptr);
	}
	else
	{
		printf("Push Failure\n");
		exit(1);
	}
}

int pop(nodeptr *insert,nodeptr *curptr)			// stack pop
{
	
	if (*insert==NULL)
	{
		printf("Pop Failure\n");
	}
	else
	{
		if ((*insert)->next != NULL)
		{
			*curptr=(*insert)->next;
		}
		else
		{
			*insert=*curptr;
			*curptr=NULL;
		}
		// printf("%d\n",(*insert)->num);// try freeing memory
		int index=(*insert)->num;
		if (*insert!=NULL)		// use free function to free memory
		{
			//*insert=NULL;
			free(*insert);
		}
		*insert=*curptr;
		//printf("curptr=%d\n",(*curptr)->num);
		return (index);
	}
}

graphptr graph_adjacent(int element)	// for making element pointer
{
	graphptr insert=(graphptr) malloc(sizeof(struct graph));
	insert->num=element;
	insert->next=NULL;
	return(insert);
}

void graph_branch(graphptr array[],int a,int b)	// giving branch to array
{
	graphptr insert=graph_adjacent(b);	// making pointer with element 'b'
	// insert->next=array[a]->head;
	// array[a]->head=insert;
	graphptr temp=array[a]->head;
	if (array[a]->head==NULL)		// joining element 'b' to leaf of 
	{					// array[a]
		array[a]->head=insert;
		insert->next=temp;
	}
	else
	{
		while (temp->next!=NULL)	// loop to get to leaf
		{
			temp=temp->next;
		}
		temp->next=insert;
		insert->next=NULL;
	}
	
	/*
	insert=graph_adjacent(a);	// making pointer with element 'a'
	// insert->next=array[b]->head;
	// array[b]->head=insert;
	temp=array[b]->head;
	if (array[b]->head==NULL)		// joining element 'a' to leaf of 
	{					// array[b]
		array[b]->head=insert;
		insert->next=temp;
	}
	else
	{
		while (temp->next!=NULL)	// loop to get to leaf
		{
			temp=temp->next;
		}
		temp->next=insert;
		insert->next=NULL;
	}
	*/
}

void graphrev_branch(graphptr arrayrev[],int a,int b)	// giving branch to array
{
	graphptr insert=graph_adjacent(b);	// making pointer with element 'b'
	// insert->next=array[a]->head;
	// array[a]->head=insert;
	graphptr temp=arrayrev[a]->head;
	if (arrayrev[a]->head==NULL)		// joining element 'b' to leaf of 
	{					// array[a]
		arrayrev[a]->head=insert;
		insert->next=temp;
	}
	else
	{
		while (temp->next!=NULL)	// loop to get to leaf
		{
			temp=temp->next;
		}
		temp->next=insert;
		insert->next=NULL;
	}

}


void dfs_explore(graphptr array[],int n,int *clock,vertex visited[],vertex pred[],vertex entry[],vertex ext[],nodeptr *insert,nodeptr *curptr,int check)
{
	(visited[n]->num)=1;		// marking for that element visited
	(entry[n]->num)=*clock;	// noting entry time
	*clock=*clock+1;
	graphptr temp=array[n]->head;
	while (temp!=NULL)	// going through array[m]->branch
	{			// and calling dfs_explore recursevely
		if ((visited[temp->num]->num)==0)
		{
			(pred[temp->num]->num)=n;	// for predecessor of element
			dfs_explore(array,temp->num,clock,visited,pred,entry,ext,insert,curptr,check);
		}
		temp=temp->next;
	}
	push(insert,curptr,n);
	// printf("$%d$",n);
	
	(ext[n]->num)=*clock;		// noting exit time
	// printf("#%d#",n);
	*clock=*clock+1;
}

// dfs on reverse graph and printing strongly connected components
void dfs_explorerev(graphptr arrayrev[],int n,vertex visited[],int temp3)
{
	if (temp3==0)
	{
		printf("%d",n);
	}
	else
	{
		printf(" %d",n);
	}
	
	(visited[n]->num)=1;		// marking for that element visited
	graphptr temp=arrayrev[n]->head;
	while (temp!=NULL)	// going through array[m]->branch
	{			// and calling dfs_explore recursevely
		if ((visited[temp->num]->num)==0)
		{
			temp3=1;
			dfs_explorerev(arrayrev,temp->num,visited,temp3);
		}
		temp=temp->next;
	}
	// printf("%d ",n);
}

void print(graphptr array[],vertex entry[],vertex ext[],vertex pred[],int n,nodeptr *curptr,nodeptr *insert,graphptr arrayrev[],int check,vertex visited[])
{
	for (int i=1 ; i<=n ; i++)		// printing outneighbours
	{
		graphptr temp=arrayrev[i]->head;
		while (temp!=NULL)	// going through array[m]->branch
		{
			printf("%d ",temp->num);
			temp=temp->next;
		}
		printf("-1\n");
	}
	// printing strongly connected components
	check=1;
	int space=0,temp3=0;
	for (int i=1 ; i<=n ; i++)
	{
		(visited[i]->num)=0;
	}
	for (int i=1 ; i<=n ; i++)
	{
		int index=pop(insert,curptr);
		// printf("#%d#",index);
		if ((*insert)==NULL)
		{
			space=1;
		}
		if ((visited[index]->num)==0 )
		{
			dfs_explorerev(arrayrev,index,visited,temp3);
			if (space==0)
			{
				printf("\n");
			}
			temp3=0;
		}
	}
	
}

void dfs(graphptr array[],int n,int m,graphptr arrayrev[],int check)
{
	int clock=0;			// clock to count the entry and exit time
	// int visited[n],pred[n],entry[n],ext[n];	// array for visited,pred,entry,exit
	
	nodeptr curptr=NULL;
	nodeptr insert=NULL;
	
	// structure difined for vertices
	vertex visited[n];
	vertex pred[n];
	vertex entry[n];
	vertex ext[n];
	
	for (int i=1 ; i<=n ; i++)
	{
		visited[i]=(vertex) malloc(sizeof(struct ver));
		pred[i]=(vertex) malloc(sizeof(struct ver));
		entry[i]=(vertex) malloc(sizeof(struct ver));
		ext[i]=(vertex) malloc(sizeof(struct ver));
		((visited[i])->num)=0;
		((pred[i])->num)=-1;
		((entry[i])->num)=-1;
		((ext[i])->num)=-1;
	}
	
	// calling dfs_explore for searching from number m
	dfs_explore(array,m,&clock,visited,pred,entry,ext,&insert,&curptr,check);
	for (int i=1 ; i<=n ; i++)		// callin for elements which 
	{					// are not connected
		if ((visited[i]->num)==0 )
		{
			dfs_explore(array,i,&clock,visited,pred,entry,ext,&insert,&curptr,check);
		}
	}
	print(array,entry,ext,pred,n,&curptr,&insert,arrayrev,check,visited);	// calling print for printing entry and exit 0 to (n-1)
}

int main()
{
	int n=0,m=0,a=0,b=0;
	scanf("%d",&n);
	int check=0;
	
	graphptr array[n];		// array for graph 
	graphptr arrayrev[n];
	for (int i=1 ; i<=n ; i++)
	{
		array[i]=(graphptr) malloc(sizeof(struct graph));
		array[i]->head=NULL;
		arrayrev[i]=(graphptr) malloc(sizeof(struct graph));
		arrayrev[i]->head=NULL;
	}
	
	for (int i=1 ; i<=n ; i++)
	{
		scanf("%d",&b);
		while (b!=-1)
		{
			graph_branch(array,i,b);		// graph G
			graphrev_branch(arrayrev,b,i);	// graph Greverse
			scanf("%d",&b);
		}
	}
	int start_num=0;		// number for starting dfs
	scanf("%d",&start_num);
	// calling depth first search
	dfs(array,n,start_num,arrayrev,check);
}
