#include "LabelRelations.h"
/*****************************************************************************/
/****************************COUNTING SORT************************************/
/*****************************************************************************/


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

