#include "globalVariables.h"
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
		nodesWithLabel[label].push_back(i);
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
		outgoingLabels[edgeList[i].u].insert(pii(nodeLabels[edgeList[i].v],edgeList[i].edgeLabel));
		incomingLabels[edgeList[i].v].insert(pii(nodeLabels[edgeList[i].u],edgeList[i].edgeLabel));
	}
	scanf("%lf",&threshold);
}
