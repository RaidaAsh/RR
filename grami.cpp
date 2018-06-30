#include "gspan.h"
/**********************************************************************/

void printFrequentSubgraphs(){
	printf("Frequent Subgraphs:\nNumber of subgraphs %d\n\n",(int)freqSubgraphs.size());
	for(int i=0; i<(int)freqSubgraphs.size(); i++){
		printSubgraph(freqSubgraphs[i]);
	}
}

int main(){
	freopen("bigIn.txt", "r", stdin);
	freopen("setout4.1.txt","w",stdout);
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
