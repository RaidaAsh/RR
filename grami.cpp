#include <bits/stdc++.h>
using namespace std;
#define NODES 1005
#define EDGES 1005
#define LABELS 1005
#define NODELABELS 1005
#define EDGELABELS 1005

typedef pair <int, int> pii;
typedef pair < pair <int, int>, int> ppi;
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
} edgeList[EDGES], distinctEdges[EDGES];

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
    vector < vector <int> > adjNode;
    vector < vector <int> > adjLabel;
    vector <int> nodeLabels;
    stack <int> rightmostPath;
};

map < ppi , double > labelRelations;

/**************** GLOBAL VARIABLES *********************************/

int numberOfNodes,numberOfEdges,numberOfNodeLabels,numberOfEdgeLabels, numberOfDistinctEdges;
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

For i=0 to number of nodes-1:
    Label(node i)

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


inline bool cmpEdges(GraphEdge a, GraphEdge b){
	if(nodeLabels[a.u]!=nodeLabels[b.u]) return nodeLabelList[nodeLabels[a.u]].indexInSortedList<nodeLabelList[nodeLabels[b.u]].indexInSortedList;
	if(nodeLabels[a.v]!=nodeLabels[b.v]) return nodeLabelList[nodeLabels[a.v]].indexInSortedList<nodeLabelList[nodeLabels[b.v]].indexInSortedList;
	if(a.edgeLabel!=b.edgeLabel) return edgeLabelList[a.edgeLabel].indexInSortedList<edgeLabelList[b.edgeLabel].indexInSortedList;
	return false;
}

inline void processEdgeList(){
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

inline bool hasSameLabels(GraphEdge a, GraphEdge b){
	return (nodeLabels[a.u]==nodeLabels[b.u] && nodeLabels[a.v]==nodeLabels[b.v] && a.edgeLabel==b.edgeLabel);
}

inline void insertDistinctEdge(GraphEdge a){
	distinctEdges[numberOfDistinctEdges]=a;
	numberOfDistinctEdges++;
}

inline void findDistinctEdges(){ //Assumes edges in EdgeList are sorted in canonical order
	numberOfDistinctEdges=0;
	insertDistinctEdge(edgeList[0]);
	for(int i=1; i<numberOfEdges; i++){
		if(!hasSameLabels(edgeList[i-1], edgeList[i])){
			insertDistinctEdge(edgeList[i]);
		}
	}
}

inline void printDistinctEdges(){ //Assumes edges in EdgeList are sorted in canonical order
	puts("");
	for(int i=0; i<numberOfDistinctEdges; i++){
		printf("%d %d %d\n", nodeLabels[distinctEdges[i].u], nodeLabels[distinctEdges[i].v], distinctEdges[i].edgeLabel); 
	}
	puts("");
}

inline void findLabelRelations(){
	labelRelations.clear();
	ppi newp;
	for(int i=0; i<numberOfEdges; i++){
		newp=ppi(pii(nodeLabels[edgeList[i].u], nodeLabels[edgeList[i].v]), edgeList[i].edgeLabel);
		labelRelations[newp]=edgeList[i].w;
	}
}

inline void printLabelRelations(){
	ppi temp;
	for(auto it=labelRelations.begin(); it!=labelRelations.end(); it++){
		temp=it->first;
		cout<<temp.first.first<<" "<<temp.first.second<<" "<<temp.second<<" "<<(it->second)<<endl; 
	}
}


Subgraphs extendBackwardEdge(Subgraphs currSubgraph, int u, int v, int edgeLabel){
	assert(v<u);
	Subgraphs newSubgraph;
	newSubgraph=currSubgraph;
	newSubgraph.numberOfEdges++;
	newSubgraph.adjNode[u].push_back(v);
	newSubgraph.adjLabel[u].push_back(edgeLabel);
	return newSubgraph;
}

Subgraphs extendForwardEdge(Subgraphs currSubgraph, int u, int v, int vLabel, int edgeLabel){
	assert(v>u);
	Subgraphs newSubgraph;
	vector <int> newVec;
	newSubgraph=currSubgraph;
	newSubgraph.numberOfNodes++;
	newSubgraph.numberOfEdges++;
	newSubgraph.adjNode[u].push_back(v);
	newSubgraph.adjLabel[u].push_back(edgeLabel);
	newSubgraph.adjNode.push_back(newVec);
	newSubgraph.adjLabel.push_back(newVec);
	newSubgraph.nodeLabels.push_back(vLabel);
	newSubgraph.rightmostPath.push(v);
	return newSubgraph;
}

void printSubgraph(Subgraphs currSubgraph)
{
	//getchar();
	printf("\n%d %d\n",currSubgraph.numberOfNodes, currSubgraph.numberOfEdges);
	for(int i = 0;i < currSubgraph.numberOfNodes;i++)
	{
		for(unsigned int j = 0;j < currSubgraph.adjNode[i].size();j++)
		{
			printf("%d %d %d\n",i,currSubgraph.adjNode[i][j],currSubgraph.adjLabel[i][j]);
		}
	}
	printf("Labels : \n");
	for(unsigned int i = 0;i< currSubgraph.nodeLabels.size();i++)
	{
		printf("%d\n",currSubgraph.nodeLabels[i]);
	}
	if(currSubgraph.rightmostPath.empty())printf("Empty\n");
	else printf("Stack top : %d\n",currSubgraph.rightmostPath.top());
}
int globalVariable = 0;
char terminator[5];
void subgraphExtension(Subgraphs currSubgraph){
	printSubgraph(currSubgraph);
	printf("Do you want to terminate?\n");
	cin>>terminator;
	if(terminator[0] == 'y')
	{
		printf("Terminated\n");
		return;
	}
	Subgraphs newSubgraph;
	int currNode;
	int low, sz;
	while(!currSubgraph.rightmostPath.empty()){
		currNode=currSubgraph.rightmostPath.top();
		sz=currSubgraph.adjNode[currNode].size();
		low=(sz==0) ? 0 : currSubgraph.adjNode[currNode][sz-1]+1;
		for(int i=low; i<currNode; i++){
			for(int j=0; j<numberOfEdgeLabels; j++){
				newSubgraph=extendBackwardEdge(currSubgraph, currNode, i, j);
				subgraphExtension(newSubgraph);
			}
		}
		for(int i=0; i<numberOfNodeLabels; i++){
			for(int j=0; j<numberOfEdgeLabels; j++){
				newSubgraph=extendForwardEdge(currSubgraph, currNode, currSubgraph.numberOfNodes, i, j);
				subgraphExtension(newSubgraph);
			}
		}
		currSubgraph.rightmostPath.pop();
	}
}
void gSpanInit()
{
	Subgraphs newSubgraph;
	for(int i = 0;i < numberOfDistinctEdges;i++)
	{
		newSubgraph.adjNode.clear();
		newSubgraph.adjLabel.clear();
		newSubgraph.nodeLabels.clear();
		while(!newSubgraph.rightmostPath.empty())newSubgraph.rightmostPath.pop();
		newSubgraph.numberOfNodes = 2;
		newSubgraph.numberOfEdges = 1;
		newSubgraph.adjNode.resize(2);
		newSubgraph.adjLabel.resize(2);
		newSubgraph.adjNode[0].push_back(1);
		newSubgraph.adjLabel[0].push_back(distinctEdges[i].edgeLabel);
		newSubgraph.rightmostPath.push(0);
		newSubgraph.rightmostPath.push(1);
		newSubgraph.nodeLabels.push_back(nodeLabels[distinctEdges[i].u]);
		newSubgraph.nodeLabels.push_back(nodeLabels[distinctEdges[i].v]);
		subgraphExtension(newSubgraph);
		printf("Done with this edge\n");
		
	}
}
int main()
{
	takeInput();
	findLabelRelations();
	printLabelRelations();
	sortNodeLabels();
	printSorted();
	sortEdgeLabels();
	printEdgeSorted();
	processEdgeList();
	printEdgeList();
	findDistinctEdges();
	printDistinctEdges();
	gSpanInit();
	return 0;
}
