#include "gspan.h"
/**********************************************************************/

int main(){
	//freopen("bigin.txt", "r", stdin);
	srand(time(NULL));
	takeInput();
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
	return 0;
}
