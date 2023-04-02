#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

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
	int dist;
	int key;
};
typedef struct HeapNode * heapnode;

int shiftup(heapnode heap_arr[],int posn,int temp_key_ind[])	// getting inserted node at correct position
{
	while(posn>0 && heap_arr[posn+1]->dist<heap_arr[((posn-1)/2)+1]->dist)
	{
		int t=heap_arr[posn+1]->dist;
		heap_arr[posn+1]->dist=heap_arr[((posn-1)/2)+1]->dist;
		heap_arr[((posn-1)/2)+1]->dist=t;
		int k=heap_arr[posn+1]->key;
		heap_arr[posn+1]->key=heap_arr[((posn-1)/2)+1]->key;
		heap_arr[((posn-1)/2)+1]->key=k;
		
		temp_key_ind[heap_arr[posn+1]->key]=((posn-1)/2)+1;
		temp_key_ind[heap_arr[((posn-1)/2)+1]->key]=posn+1;
		
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
		if ((2*posn)+1<=heapsize-1 && heap_arr[(2*posn)+1+1]->dist<heap_arr[posn+1]->dist)
		{
			smallest=(2*posn)+1;
		}
		else
		{
			smallest=posn;
		}
		if ((2*posn)+2<=heapsize-1 && heap_arr[(2*posn)+2+1]->dist<heap_arr[smallest+1]->dist)
		{
			smallest=(2*posn)+2;
		}
		if (smallest==posn)
		{
			return(0);
		}
		else
		{
			int t=heap_arr[posn+1]->dist;		// swap distance
			heap_arr[posn+1]->dist=heap_arr[smallest+1]->dist;
			heap_arr[smallest+1]->dist=t;
			int k=heap_arr[posn+1]->key;		// swap key
			heap_arr[posn+1]->key=heap_arr[smallest+1]->key;
			heap_arr[smallest+1]->key=k;
			
			temp_key_ind[heap_arr[posn+1]->key]=smallest+1;
			temp_key_ind[heap_arr[smallest+1]->key]=posn+1;
			
			minheapify(heap_arr,smallest,heapsize,temp_key_ind);
		}
	}
}

void extractmin(heapnode heap_arr[],int *n,int temp_key_ind[])	// delete least element
{
	//printf("%d",heap_arr[1]->key);
	heap_arr[1]->dist=heap_arr[*n]->dist;
	heap_arr[1]->key=heap_arr[*n]->key;
	temp_key_ind[heap_arr[*n]->key]=1;
	*n=*n-1;
	minheapify(heap_arr,0,*n,temp_key_ind);
}

int decreasekey(heapnode heap_arr[],int posn,int x,int heapsize,int temp_key_ind[])	// increase priority of given posn
{								// given index
	if(heap_arr[posn+1]->dist<=x || posn>heapsize-1)
	{
		//printf("Operation Fail\n");
		return(0);
	}
	else
	{
		heap_arr[posn+1]->dist=x;
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
	// structure difined for vertices
	vertex pred[n+1];
	int size=n;
	
	for (int i=1 ; i<=n ; i++)
	{
		pred[i]=(vertex) malloc(sizeof(struct ver));
		((pred[i])->num)=-1;
	}
	int temp_arr[n+1];		// has dist value corresponding key
	int temp_key_ind[n+1];	// has index value corresponding key
	heapnode heap_arr[n+1];
	heap_arr[1]=(heapnode) malloc(sizeof(struct HeapNode));	// initialising heap_arr
	heap_arr[1]->dist=0;
	heap_arr[1]->key=m;
	temp_arr[m]=0;
	temp_key_ind[m]=1;
	int keys=0;
	for (int i=2 ; i<=n ; i++)
	{
		keys+=1;
		heap_arr[i]=(heapnode) malloc(sizeof(struct HeapNode));
		heap_arr[i]->dist=2000000000;
		if (keys==m){
			keys+=1;
		}
		temp_arr[keys]=2000000000;
		temp_key_ind[keys]=i;
		heap_arr[i]->key=keys;
	}
	//printf("hello1\n");
	while (n>0){
		int index=heap_arr[1]->key;
		int min_d=heap_arr[1]->dist;
		extractmin(heap_arr,&n,temp_key_ind);
		graphptr temp=array[index]->head;
		//printf("hello2 %d %d\n",index,min_d);
		while (temp!=NULL)
		{
			// temp->num has the value
			// temp->edge_weight ==> w(vu)
			// heap_arr[] u==>temp_arr[temp->num] 
			//			  v==>min_d
			//printf("==> %d + %d <or> %d source:%d key:%d\n",temp_arr[index],temp->edge_weight,temp_arr[temp->num],index,temp->num);
			if (temp_arr[index] + temp->edge_weight < temp_arr[temp->num]){	// edge relaxation
				int posn=temp_key_ind[temp->num];
				int x=temp_arr[index] + temp->edge_weight;
				decreasekey(heap_arr,posn-1,x,n,temp_key_ind);
				//printf("changed\n");
				pred[temp->num]->num=index;
				temp_arr[temp->num]=x;
			}
			temp=temp->next;
		}
	}
	for (int i=1; i<=size; i++){	// printing shortest distance and it's predecessors
		if (temp_arr[i]==2000000000){
			printf("Unreachable\n");	
		}else{
			printf("%d %d\n",temp_arr[i],pred[i]->num);
		}
	}
}

int main()
{
	int n=0,m=0,a=0,b=0,e_weight=0;
	scanf("%d",&n);

	graphptr array[n+1];		// array for graph 
	
	
	for (int i=1 ; i<=n ; i++)
	{
		array[i]=(graphptr) malloc(sizeof(struct graph));
		array[i]->head=NULL;
	}
	
	for (int i=1 ; i<=n ; i++)
	{
		scanf("%d",&b);
		while (b!=-1)
		{
			scanf("%d",&e_weight);
			graph_branch(array,i,b,e_weight);
			scanf("%d",&b);
		}
	}

	// int dag=0;
	int start_num=0;		// number for starting dfs
	scanf("%d",&start_num);
	// calling depth first search
	dfs(array,n,start_num);
}
