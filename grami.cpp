#include "gspan.h"
/**********************************************************************/

void printFrequentSubgraphs(){
	printf("Frequent Subgraphs:\nNumber of subgraphs %d\n\n",(int)freqSubgraphs.size());
	for(int i=0; i<(int)freqSubgraphs.size(); i++){
		printSubgraph(freqSubgraphs[i]);
	}
}

int main(){
	freopen("inTest.txt", "r", stdin);
	freopen("outTest.txt","w",stdout);
	//srand(time(NULL));
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
