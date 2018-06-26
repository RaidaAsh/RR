#include "DistinctLabelledEdgeFinder.h"
CspVariables cspVar[NODES];
bool cmpCspVar(CspVariables a, CspVariables b)
{
	return (a.domainSize < b.domainSize)||(a.domainSize == b.domainSize && a.numberOfAdjEdges > b.numberOfAdjEdges);
	
}
void printCSPVariables(Subgraphs sub)
{
	printf("\nPrinting CSP Variables :\n");
	for(int i = 0; i < sub.numberOfNodes;i++)
	{
		printf("%d %d %d\n",cspVar[i].discoveryTime,cspVar[i].domainSize,cspVar[i].numberOfAdjEdges);
	}
}

CSP findDomain(int edgeIndex)
{
	CSP newCSP;
	newCSP.domain.resize(2);
	GraphEdge currEdge = edgeList[edgeIndex];
	int labelu=nodeLabels[currEdge.u];
	int labelv=nodeLabels[currEdge.v];
	int sz = nodesWithLabel[labelu].size();
	int currNode;
	for(int i = 0;i < sz; i++)
	{
		currNode = nodesWithLabel[labelu][i];
		if(outgoingLabels[currNode].find(pii(nodeLabels[currEdge.v],currEdge.edgeLabel)) != outgoingLabels[currNode].end())
		{
			newCSP.domain[0].push_back(currNode);
		}
	}
	sz = nodesWithLabel[labelv].size();
	for(int i = 0;i < sz; i++)
	{
		currNode = nodesWithLabel[labelv][i];
		if(incomingLabels[currNode].find(pii(nodeLabels[currEdge.u],currEdge.edgeLabel)) != incomingLabels[currNode].end())
		{
			newCSP.domain[1].push_back(currNode);
		}
	}
	newCSP.mnSize=min(newCSP.domain[1].size(), newCSP.domain[0].size());
	return newCSP;           
}

bool isFrequent(Subgraphs sub, CSP csp)
{
	for(int i = 0;i < sub.numberOfNodes;i++)
	{
		cspVar[i].discoveryTime = i;
		cspVar[i].domainSize = csp.domain[i].size();
		cspVar[i].numberOfAdjEdges = sub.adjNode[i].size() + sub.incomingAdjNode[i].size();
	}
	sort(cspVar,cspVar+sub.numberOfNodes,cmpCspVar);
	printCSPVariables(sub);
	
	return true;
}
