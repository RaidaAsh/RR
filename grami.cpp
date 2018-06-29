#include "gspan.h"
/**********************************************************************/

void printFrequentSubgraphs(){
	printf("Frequent Subgraphs:\n");
	for(int i=0; i<(int)freqSubgraphs.size(); i++){
		printSubgraph(freqSubgraphs[i]);
	}
}

int main(){
	//freopen("bigin.txt", "r", stdin);
	freopen("out.txt","w",stdout);
	srand(time(NULL));
	takeInput();
	//printIncomingNodes();
	//printOutgoingNodes();
	//printNodesWithLabel();
	findLabelRelations();
	//printLabelRelations();
	sortNodeLabels();
	//printSorted();
	sortEdgeLabels();
	//printEdgeSorted();
	processEdgeList();
	//printEdgeList();


	findDistinctEdges();
	//printDistinctEdges();
	//printIncomingLabels();
	gSpanInit();
	printFrequentSubgraphs();
	return 0;
}
