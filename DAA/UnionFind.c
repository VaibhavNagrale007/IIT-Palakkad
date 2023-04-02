#include<stdio.h>

int Find(int Parent[],int x){
	int y=x;
	while (Parent[y]!=y){	// 1st while : to find the representative element
		y=Parent[y];
	}
	int z=x;
	while (Parent[z]!=z){	// 2nd while : for path compression
		int p=Parent[z];
		Parent[z]=y;
		z=p;
	}
	return y;
}

int Union(int Parent[],int Rank[],int x,int y){
	int p1=Find(Parent,x);
	int p2=Find(Parent,y);
	if (p1==p2){			// if both have same rep.element then nothing to do
		return 1;
	}else{
		int r1=Rank[p1];
		int r2=Rank[p2];
		if (r1==r2){		// if equal just do their union 
			Parent[p2]=p1;
			Rank[p1]=Rank[p1]+1;
		}
		else if (r1<r2){	// else joining smaller set to larger one
			Parent[p1]=p2;
		}
		else{
			Parent[p2]=p1;
		}
	}
}

void Print(int Parent[],int n){
	for (int i=1; i<=n; i++){		// printing element and its representative
		printf("%d %d\n",i,Find(Parent,Parent[i]));
	}
}

int main(){
	int n=0,branch=0,x=0,y=0;
	scanf("%d",&n);
	int Parent[n],Rank[n];		// array of Parent and Rank
	// Make Set
	for (int i=1; i<=n; i++){	// make set operation
		Parent[i]=i;
		Rank[i]=0;
	}
	while (1){
		scanf("%d",&branch);
		if (branch==1){			// printf("find\n");// Find
			scanf("%d",&x);
			printf("%d\n",Find(Parent,x));
		}else if (branch==2){	// printf("union\n");// Union
			scanf("%d%d",&x,&y);
			Union(Parent,Rank,x,y);
			int temp=Find(Parent,x);	// or temp=Parent[x]
			printf("%d %d\n",temp,Rank[temp]);
		}else if (branch==3){	// printf("print\n");// Print
			Print(Parent,n);
		}else if (branch==4){	// Exit
			break;
		}else{					// Default
			printf("Invalid Command\n");
			// break;
		}
	}
}