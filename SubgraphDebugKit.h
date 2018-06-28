#include "SubgraphEdgeExtender.h"

void printCSP(CSP csp){
	printf("CSP:\n");
	printf("Min Size=%d\n", csp.mnSize);
	for(int i=0; i<(int)csp.domain.size(); i++){
		printf("%d: ", i); 
		for(int j=0; j<(int)csp.domain[i].size(); j++){
			printf("%d ", csp.domain[i][j]);
		}
		puts("");
	}
	puts("");
	puts("");
	puts("");
}

void printSubgraph(Subgraphs currSubgraph)
{
	printf("Subgraph:\n");
	//getchar();
	printf("\nNodes in subgraph: %d\n Edges in Subgraph: %d\n",currSubgraph.numberOfNodes, currSubgraph.numberOfEdges);
	printf("\nThe edges in the subgraph are\n"); 
	for(int i = 0;i < currSubgraph.numberOfNodes;i++)
	{
		for(unsigned int j = 0;j < currSubgraph.adjNode[i].size();j++)
		{
			printf("%d %d %d\n",i,currSubgraph.adjNode[i][j],currSubgraph.adjLabel[i][j]);
		}
	}
	printf("Labels of the nodes: ");
	for(unsigned int i = 0;i< currSubgraph.nodeLabels.size();i++)
	{
		printf("%d ",currSubgraph.nodeLabels[i]);
	}
	puts("");
	if(currSubgraph.rightmostPath.empty())printf("Empty\n");
	else printf("Stack top : %d\n",currSubgraph.rightmostPath.top());
}

bool inAdjEdges(Subgraphs sub, int u, int v, int elabel){
	for(int i=0; i<(int)sub.adjNode[u].size(); i++){
		if(sub.adjNode[u][i]==v && sub.adjLabel[u][i]==elabel) return true;
	}
	return false;
}

bool inInEdges(Subgraphs sub, int u, int v, int elabel){
	for(int i=0; i<(int)sub.incomingAdjNode[u].size(); i++){
		if(sub.incomingAdjNode[u][i]==v && sub.incomingAdjLabel[u][i]==elabel) return true;
	}
	return false;
}

bool incomingEdgesConsistent(Subgraphs sub){
	for(int i=0; i<sub.numberOfNodes; i++){
		for(int j=0; j<(int)sub.incomingAdjNode[i].size(); j++){
			if(!inAdjEdges(sub, sub.incomingAdjNode[i][j], i, sub.incomingAdjLabel[i][j])){
				return false;
			}
		}
	}
	return true;
}

bool edgesConsistent(Subgraphs sub){
	for(int i=0; i<sub.numberOfNodes; i++){
		for(int j=0; j<(int)sub.adjNode[i].size(); j++){
			if(!inInEdges(sub, sub.adjNode[i][j], i, sub.adjLabel[i][j])){
				return false;
			}
		}
	}
	return true;
}

bool domainConsistent(CSP csp){
	for(int i=0; i<(int)csp.domain.size(); i++){
		for(int j=0; j<(int)csp.domain[i].size(); j++){
			if(csp.domain[i][j]<0 || csp.domain[i][j]>=numberOfNodes) return false;
		}
	}
	return true;
}

