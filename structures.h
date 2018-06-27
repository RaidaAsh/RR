#include "definitions.h"

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
vector <int> nodesWithLabel[NODELABELS];
struct EdgeLabels{
        int label;
        int count; //Number of edges with corresponding label 
        int indexInSortedList;
}edgeLabelList[EDGELABELS],sortedEdgeLabels[EDGELABELS];

    
struct Subgraphs{
	int numberOfNodes;
	int numberOfEdges;
	vector <vector <int> > incomingAdjNode;
	vector <vector <int> > incomingAdjLabel;
    vector < vector <int> > adjNode;
    vector < vector <int> > adjLabel;
    vector <int> nodeLabels;
    stack <int> rightmostPath;
};

struct CSP{
	unsigned int mnSize;
	vector< vector<int>> domain;
};

struct CspVariables{
	int domainSize;
	int numberOfAdjEdges;
	int discoveryTime;
};


struct CSPValues{
	int value;
	int constrainingDegree;
};
