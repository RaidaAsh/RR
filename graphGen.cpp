#include <bits/stdc++.h>
using namespace std;
/*
 * 
 * 
Number of Nodes
Number of Edges
Number of Node Labels
Number of Edge Labels

For i=0 to number of nodes-1:
    Label(node i)

For i = 0 to number of egdes -1:
	
	Node1(u)
	Node2(v)
	Weight(w)
	Edge Label(edgeLabel)
	
Threshold
* 
* 
*/
#define NODES 200
#define EDGES 2000
#define NODELABELS 10
#define EDGELABELS 10
int nodeLabels[NODES];
int edgeLabels[EDGES];
int taken[NODES][NODES][EDGELABELS];

int genRandomWithin(int a, int b){	
	return rand()%(b-a+1)+a;
}

int main()
{
	srand(time(NULL));
	freopen("in1.txt","w",stdout);
	int numberOfNodes = genRandomWithin(20, 80);
	int numberOfEdges = genRandomWithin(numberOfNodes*sqrt(numberOfNodes), numberOfNodes*numberOfNodes);
	int numberOfNodeLabels = genRandomWithin(3, 9);
	int numberOfEdgeLabels = genRandomWithin(2, 5);
	printf("%d %d\n",numberOfNodes,numberOfEdges);
	printf("%d %d\n",numberOfNodeLabels,numberOfEdgeLabels);
	int edgeLabel;
	memset(nodeLabels, -1, sizeof(nodeLabels));
	for(int i=0; i<numberOfNodeLabels; i++){
		int pos=rand()%numberOfNodes;
		nodeLabels[pos]=i;
	}
	for(int i = 0;i < numberOfNodes;i++)
	{
		if(nodeLabels[i]==-1) nodeLabels[i] = rand()%numberOfNodeLabels;
		printf("%d ",nodeLabels[i]);
	}
	printf("\n");
	
	memset(edgeLabels, -1, sizeof(edgeLabels));
	for(int i=0; i<numberOfEdgeLabels; i++){
		int pos=rand()%numberOfEdges;
		edgeLabels[pos]=i;
	}
	
	for(int i = 0;i< numberOfEdges;i++)
	{
		int node1,node2;
		while(true){
			node1 = rand() % numberOfNodes;
			node2 = rand() % numberOfNodes;
			if(edgeLabels[i]==-1) edgeLabel = rand() % numberOfEdgeLabels;
			else{
				edgeLabel=edgeLabels[i];
			}
			if(!taken[node1][node2][edgeLabel])break;
		}
		double weight = nodeLabels[node1] + nodeLabels[node2]/10.0 + edgeLabel/100.0;
		printf("%d %d %lf %d\n",node1,node2,weight,edgeLabel);
	}
	printf("%lf\n",(double)(genRandomWithin(5, 20)));
	return 0;
}
