#include "takeInput.h"

/**************************************************************************************************/
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

void printNodesWithLabel()
{
	printf("Nodes with Label:\n");
	for(int i = 0;i<numberOfNodeLabels;i++)
	{
		printf("Node Label : %d \n",i);
		for(int j = 0;j < (int) nodesWithLabel[i].size();j++)
		{
			printf("%d ",nodesWithLabel[i][j]);
		}
		printf("\n");
	}
}
void printIncomingLabels()
{
	printf("Incoming Labels\n");
	for(int i = 0;i<numberOfNodes;i++)
	{
		set <pii> :: iterator it;
		for( it = incomingLabels[i].begin();it!= incomingLabels[i].end();it++)
		{
			printf("%d -> %d : %d\n",i,it->first,it->second);
		}
	}
}

void printIncomingNodes(){
	printf("Incoming Nodes\n");
	for(int i = 0;i<numberOfNodes;i++)
	{
		set <pii> :: iterator it;
		for( it = incomingNodes[i].begin();it!= incomingNodes[i].end();it++)
		{
			printf("%d -> %d : %d\n",i,it->first,it->second);
		}
	}
}

void printOutgoingNodes(){
	printf("Outgoing Nodes\n");
	for(int i = 0;i<numberOfNodes;i++)
	{
		set <pii> :: iterator it;
		for( it = outgoingNodes[i].begin();it!= outgoingNodes[i].end();it++)
		{
			printf("%d -> %d : %d\n",i,it->first,it->second);
		}
	}
}
