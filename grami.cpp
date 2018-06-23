#include <bits/stdc++.h>
using namespace std;
#define NODES 1005
#define EDGES 1005
#define LABELS 1005
#define NODELABELS 1005
#define EDGELABELS 1005

#define mp make_pair
#define pr pair<int, double>
#define mx 100005


/***************************Structures*************************************/

struct GraphEdge{
    int u, v; //An edge connecting node u to node v (u->v if directed)
    double w;
    int edgeLabel; //label before sorting
    GraphEdge(){}
    GraphEdge(int _u,int _v, double _w, int _edgeLabel)
    {
		u = _u;
		v = _v;
		w = _w;
		edgeLabel = _edgeLabel;
	}
} edgeList[EDGES];

int nodeLabels[NODES];

struct NodeLabels{
        int label;
        int count; //Number of nodes with corresponding label 
		int indexInSortedList;
}nodeLabelList[NODELABELS], sortedNodeLabels[NODELABELS];

struct EdgeLabels{
        int label;
        int count; //Number of edges with corresponding label 
        int indexInSortedList;
}edgeLabelList[EDGELABELS],sortedEdgeLabels[EDGELABELS];

    
struct Subgraphs{
	int numberOfNodes;
	int numberOfEdges;
    vector < vector <int> > adjList;
    vector <int> nodeLabels;
    stack <int> nodeStack;
};


/**************** GLOBAL VARIABLES *********************************/

int numberOfNodes,numberOfEdges,numberOfNodeLabels,numberOfEdgeLabels;
vector <int> adj[NODES]; // adjacency list
int startIterator[NODES];
double threshold;
vector <Subgraphs> freqSubgraphs;

/**************************INPUT FORMAT**********************************/

/*~~~~~~~~~~~~~~~~~~~~~~~~ 0 based ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*
 * 
 * 
Number of Nodes
Number of Edges
Number of Node Labels
Number of Edge Labels

For i = 0 to number of egdes -1:
	
	Node1(u)
	Node2(v)
	Weight(w)
	Edge Label(edgeLabel)
	
Threshhold
* 
* 
*/
/*****************************************************************************/
/****************************COUNTING SORT************************************/
/*****************************************************************************/

int freq[NODES + EDGES],taken[NODES + EDGES];
/************************SORT NODES *****************************************/

void sortNodeLabels()
{
	memset(freq,0,sizeof(freq));
	memset(taken,0,sizeof(taken));
	for(int i = 0;i < numberOfNodeLabels; i++)
	{
		freq[nodeLabelList[i].count]++;
	}
	for(int i = numberOfNodes ;i>=0 ; i--)
	{
		freq[i] += freq[i+1];
	}
	for(int i = 0;i <numberOfNodeLabels; i++)
	{
		sortedNodeLabels[freq[nodeLabelList[i].count + 1] + taken[nodeLabelList[i].count]] = nodeLabelList[i];
		nodeLabelList[i].indexInSortedList = freq[nodeLabelList[i].count + 1] + taken[nodeLabelList[i].count];
		taken[nodeLabelList[i].count]++;
	}
}


void printSorted(){
	for(int i=0; i<numberOfNodeLabels; i++){
		printf("%d %d\n", sortedNodeLabels[i].label, sortedNodeLabels[i].count);
	}
	printf("\n");
	for(int i = 0;i<numberOfNodeLabels;i++)
	{
		printf("%d\n",nodeLabelList[i].indexInSortedList);
	}
	printf("\n");
}

/************************ SORT EDGES *****************************************/
void sortEdgeLabels()
{
	memset(freq,0,sizeof(freq));
	memset(taken,0,sizeof(taken));
	for(int i = 0;i < numberOfEdgeLabels; i++)
	{
		freq[edgeLabelList[i].count]++;
	}
	for(int i = numberOfEdges ;i>=0 ; i--)
	{
		freq[i] += freq[i+1];
	}
	for(int i = 0;i <numberOfEdgeLabels; i++)
	{
		sortedEdgeLabels[freq[edgeLabelList[i].count + 1] + taken[edgeLabelList[i].count]] = edgeLabelList[i];
		edgeLabelList[i].indexInSortedList = freq[edgeLabelList[i].count + 1] + taken[edgeLabelList[i].count];
		taken[edgeLabelList[i].count]++;
	}
}


void printEdgeSorted(){
	for(int i=0; i<numberOfEdgeLabels; i++){
		printf("%d %d\n", sortedEdgeLabels[i].label, sortedEdgeLabels[i].count);
	}
	printf("\n");
	for(int i = 0;i<numberOfEdgeLabels;i++)
	{
		printf("%d\n",edgeLabelList[i].indexInSortedList);
	}
	printf("\n");
}

/*****************************************************************************/
/****************************TAKE INPUT***************************************/
/*****************************************************************************/
void takeInput()
{
	int label;
	scanf("%d %d",&numberOfNodes,&numberOfEdges);
	scanf("%d %d",&numberOfNodeLabels,&numberOfEdgeLabels);
	for(int i = 0;i < numberOfNodeLabels;i++)
	{
		nodeLabelList[i].label = i;
		nodeLabelList[i].count = 0;
	}
	for(int i = 0;i < numberOfNodes;i++)
	{
		scanf("%d",&label);
		assert(label<NODELABELS && label>= 0);
		nodeLabelList[label].count++;
		nodeLabels[i]=label;
	}
	
	for(int i = 0;i < numberOfEdgeLabels;i++)
	{
		edgeLabelList[i].label = i;
		edgeLabelList[i].count = 0;
	}
	for(int i = 0;i < numberOfEdges;i++)
	{
		scanf("%d %d %lf %d",&edgeList[i].u,&edgeList[i].v,&edgeList[i].w,&edgeList[i].edgeLabel);
		edgeLabelList[edgeList[i].edgeLabel].count++;
	}
	scanf("%lf",&threshold);
}
/**********************************************************************/


bool cmpEdges(GraphEdge a, GraphEdge b){
	if(nodeLabels[a.u]!=nodeLabels[b.u]) return nodeLabelList[nodeLabels[a.u]].indexInSortedList<nodeLabelList[nodeLabels[b.u]].indexInSortedList;
	if(nodeLabels[a.v]!=nodeLabels[b.v]) return nodeLabelList[nodeLabels[a.v]].indexInSortedList<nodeLabelList[nodeLabels[b.v]].indexInSortedList;
	if(a.edgeLabel!=b.edgeLabel) return edgeLabelList[a.edgeLabel].indexInSortedList<edgeLabelList[b.edgeLabel].indexInSortedList;
	return false;
}

void processEdgeList(){
	sort(edgeList, edgeList+numberOfEdges, cmpEdges);
	for(int i=0; i<numberOfEdges; i++){
		adj[edgeList[i].u].push_back(i);
	}
	memset(startIterator, 0, sizeof(startIterator));
}

void printEdgeList(){
	for(int i=0; i<numberOfEdges; i++){
		printf("%d %d %f %d\n", edgeList[i].u, edgeList[i].v, edgeList[i].w, edgeList[i].edgeLabel);
	}
}

void subgraphExtension(Subgraphs curr)
{
	
}
int main()
{
	takeInput();
	sortNodeLabels();
	printSorted();
	sortEdgeLabels();
	printEdgeSorted();
	processEdgeList();
	printEdgeList();
	return 0;
}
