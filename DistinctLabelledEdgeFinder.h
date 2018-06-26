#include "LargeGraphEdgeSort.h"

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

inline void printDistinctEdges(){
	puts("");
	for(int i=0; i<numberOfDistinctEdges; i++){
		printf("%d %d %d\n", nodeLabels[distinctEdges[i].u], nodeLabels[distinctEdges[i].v], distinctEdges[i].edgeLabel); 
	}
	puts("");
}


