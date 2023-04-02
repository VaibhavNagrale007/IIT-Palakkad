#include<stdio.h>
#include<stdlib.h>

struct graph	// structure for graph
{
	int num;
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

struct HeapNode		// structure for heap
{
	int weight;
	int key;
};
typedef struct HeapNode * heapnode;

int shiftup(heapnode heap_arr[],int posn,int temp_key_ind[])	// getting inserted node at correct position
{
	//printf("OUT SUP key: %d dist : %d\n",heap_arr[posn]->key,heap_arr[posn]->weight);
	while(posn>0 && heap_arr[posn]->weight<heap_arr[((posn-1)/2)]->weight)
	{
		int t=heap_arr[posn]->weight;
		heap_arr[posn]->weight=heap_arr[((posn-1)/2)]->weight;
		heap_arr[((posn-1)/2)]->weight=t;
		
		temp_key_ind[heap_arr[posn]->key]=((posn-1)/2);
		temp_key_ind[heap_arr[((posn-1)/2)]->key]=posn;
		
		int k=heap_arr[posn]->key;
		heap_arr[posn]->key=heap_arr[((posn-1)/2)]->key;
		heap_arr[((posn-1)/2)]->key=k;
		//printf("IN SHIFT UP key: %d dist: %d\n",k,t);
		
		posn=(posn-1)/2;
	}
}

int minheapify(heapnode heap_arr[],int posn,int heapsize,int temp_key_ind[])	// place heap[posn] at correct posn
{
	int smallest=0;
	// left=(2*posn)+1
	// right=(2*posn)+2
	if ((2*posn)+1>heapsize-1)
	{
		return(0);
	}
	else
	{
		if ((2*posn)+1<=heapsize-1 && heap_arr[(2*posn)+1]->weight<heap_arr[posn]->weight)
		{
			smallest=(2*posn)+1;
		}
		else
		{
			smallest=posn;
		}
		if ((2*posn)+2<=heapsize-1 && heap_arr[(2*posn)+2]->weight<heap_arr[smallest]->weight)
		{
			smallest=(2*posn)+2;
		}
		if (smallest==posn)
		{
			return(0);
		}
		else
		{
			int t=heap_arr[posn]->weight;		// swap distance
			heap_arr[posn]->weight=heap_arr[smallest]->weight;
			heap_arr[smallest]->weight=t;
			
			temp_key_ind[heap_arr[posn]->key]=smallest;
			temp_key_ind[heap_arr[smallest]->key]=posn;
			
			int k=heap_arr[posn]->key;		// swap key
			heap_arr[posn]->key=heap_arr[smallest]->key;
			heap_arr[smallest]->key=k;
			
			minheapify(heap_arr,smallest,heapsize,temp_key_ind);
		}
	}
}

void extractmin(heapnode heap_arr[],int *n,int temp_key_ind[])	// delete least element
{
	//printf("@@@@@@@\n");
	heap_arr[0]->weight=heap_arr[*n-1]->weight;
	heap_arr[0]->key=heap_arr[*n-1]->key;
	temp_key_ind[heap_arr[*n-1]->key]=0;
	*n=*n-1;
	minheapify(heap_arr,0,*n,temp_key_ind);
}

int decreasekey(heapnode heap_arr[],int posn,int x,int heapsize,int temp_key_ind[])	// increase priority of given posn
{								// given index
	if(heap_arr[posn]->weight<=x || posn>heapsize-1)
	{
		//printf("Operation Fail\n");
		return(0);
	}
	else
	{
		heap_arr[posn]->weight=x;
		//printf("IN DEC KEY dist: %d key %d posn %d\n",x,heap_arr[posn]->key,posn);
		shiftup(heap_arr,posn,temp_key_ind);
	}
}

graphptr graph_adjacent(int element,int e_weight)	// for making element pointer
{
	graphptr insert=(graphptr) malloc(sizeof(struct graph));
	insert->num=element;
	insert->edge_weight=e_weight;
	//printf("%d %d\n",insert->num,insert->edge_weight);
	insert->next=NULL;
	return(insert);
}

void graph_branch(graphptr array[],int a,int b,int e_weight)	// giving branch to array
{
	graphptr insert=graph_adjacent(b,e_weight);	// making pointer with element 'b'
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
		//printf("%d %d\n",temp->num,temp->edge_weight);
	}
	
}

void dfs(graphptr array[],int n,int m)
{
	// structure difined for prev element
	vertex prev[n];
	int size=n;
	int visited[n];		// for checking already visited vertices
	for (int i=0 ; i<n ; i++)
	{
		prev[i]=(vertex) malloc(sizeof(struct ver));
		((prev[i])->num)=-1;
		visited[i]=0;
	}
	int temp_arr_weight[n];		// has dist value corresponding key
	int temp_key_ind[n];		// has index value corresponding key
	heapnode heap_arr[n];		// our minheap and it's initialization
	heap_arr[0]=(heapnode) malloc(sizeof(struct HeapNode));
	heap_arr[0]->weight=0;
	heap_arr[0]->key=m;
	temp_arr_weight[m]=0;
	temp_key_ind[m]=0;
	int keys=0;
	for (int i=1 ; i<n ; i++)
	{
		heap_arr[i]=(heapnode) malloc(sizeof(struct HeapNode));
		heap_arr[i]->weight=2000000000;
		if (keys==m){
			keys+=1;
		}
		temp_arr_weight[keys]=2000000000;
		temp_key_ind[keys]=i;
		heap_arr[i]->key=keys;
		keys+=1;
	}
	
	// going through adjacent elements of our starting vertex
	extractmin(heap_arr,&n,temp_key_ind);
	graphptr temp=array[m]->head;
	while (temp!=NULL){
		if (temp->edge_weight < temp_arr_weight[temp->num]){	// edge relaxation
			int posn=temp_key_ind[temp->num];
			int x=temp->edge_weight;
			decreasekey(heap_arr,posn,x,n,temp_key_ind);
			prev[temp->num]->num=m;
			// printf("temp->num %d pred %d wt %d\n",temp->num+1,m+1,x);
			temp_arr_weight[temp->num]=x;
		}
		temp=temp->next;
	}
	visited[m]=1;	// 1st element is visited
	
	int S[n],p=0;	// array to store extracted elements
	while (n>0){
		int index=heap_arr[0]->key;
		int min_w=heap_arr[0]->weight;	// minimum weight and it's key value
		S[p]=index;p+=1;
		extractmin(heap_arr,&n,temp_key_ind);	// extract and make it visited
		graphptr temp=array[index]->head;
		visited[index]=1;
		
		// go through it's adjacent elements and change weight
		while (temp!=NULL)
		{
			if (temp->edge_weight < temp_arr_weight[temp->num] && visited[temp->num]!=1){
				int posn=temp_key_ind[temp->num];
				int x=temp->edge_weight;
				decreasekey(heap_arr,posn,x,n,temp_key_ind);
				prev[temp->num]->num=index;
				//printf("temp->num %d pred %d wt %d\n",temp->num+1,index+1,x);
				temp_arr_weight[temp->num]=x;
			}
			temp=temp->next;
		}
	}
	for (int i=0; i<size-1; i++){
		printf("%d %d %d\n",prev[S[i]]->num+1,S[i]+1,temp_arr_weight[S[i]]);
		// printing ::::: prev of S[i] and S[i] (the edge) and weight of edge
	}
}

int main()
{
	int n=0,m=0,a=0,b=0,e_weight=0;
	scanf("%d",&n);

	graphptr array[n];		// array for graph 
	
	
	for (int i=0 ; i<n ; i++)
	{
		array[i]=(graphptr) malloc(sizeof(struct graph));
		array[i]->head=NULL;
	}
	
	for (int i=0 ; i<n ; i++)
	{
		scanf("%d",&b);
		while (b!=-1)
		{
			scanf("%d",&e_weight);
			graph_branch(array,i,b-1,e_weight);
			scanf("%d",&b);
		}
	}

	// int dag=0;
	int start_num=0;		// number for starting dfs
	scanf("%d",&start_num);
	// calling depth first search
	dfs(array,n,start_num-1);
}
