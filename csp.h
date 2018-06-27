#include "SubgraphDebugKit.h"

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

int sol[NODES];
bool domainTested[NODES];
bool isAssigned[NODES];
set <pii> marked;
CSP searchCSP;
Subgraphs searchSubgraph;
int node;
CSPValues domainOrder[NODES];


bool cmpCSPValues(CSPValues a, CSPValues b){
	return a.constrainingDegree<b.constrainingDegree;
}

int constrainingDegreeOfValue(int value){ //Number of additional constraints imposed by assigining value to node
	int count=0;
	for(int i=0; i<(int)searchCSP.domain.size(); i++){
		for(int j=0; j<(int)searchCSP.domain[i].size(); j++){
			if(searchCSP.domain[i][j]==value){
				count++;
				break;
			}
		}
	}
	int v;
	for(int i=0; i<(int)searchSubgraph.adjNode[node].size(); i++){
		v=searchSubgraph.adjNode[node][i];
		for(int j=0; j<(int)searchCSP.domain[v].size(); j++){
			if(outgoingNodes[searchCSP.domain[v][j]].find(pii(value, searchSubgraph.adjLabel[node][i]))==outgoingNodes[searchCSP.domain[v][j]].end())
				count++;
		}
	}
	
	for(int i=0; i<(int)searchSubgraph.incomingAdjNode[node].size(); i++){
		v=searchSubgraph.incomingAdjNode[node][i];
		for(int j=0; j<(int)searchCSP.domain[v].size(); j++){
			if(incomingNodes[searchCSP.domain[v][j]].find(pii(value, searchSubgraph.incomingAdjLabel[node][i]))==incomingNodes[searchCSP.domain[v][j]].end())
				count++;
		}
	}
	return count;
}



inline int findNextVariable(bool fg){ //Returns most constrained, most constraining variable
	int mn=-1;
	for(int i=0; i<searchSubgraph.numberOfNodes; i++){
		if((fg && !domainTested[i]) || (!fg && !isAssigned[i])){
			if(mn==-1){
				mn=i;
			}
			else if(searchCSP.domain[i].size()<searchCSP.domain[mn].size()){
				mn=i; //Most constrained variable
			}
			else if(searchCSP.domain[i].size()==searchCSP.domain[mn].size() && (searchSubgraph.adjNode[i].size()+searchSubgraph.incomingAdjNode[i].size())>(searchSubgraph.adjNode[mn].size()+searchSubgraph.incomingAdjNode[mn].size())){
				mn=i; //Most constraining value
			}
		}
	}
	return mn;	
}



bool search(int assignedNodes){
	if(assignedNodes==searchSubgraph.numberOfNodes) return true;
	int nxt=findNextVariable(false);
	if(searchCSP.domain[nxt].size()==0) return false;
	int sz=searchCSP.domain[nxt].size();
	for(int i=0; i<sz; i++){
		domainOrder[i].value=searchCSP.domain[nxt][i];
		domainOrder[i].constrainingDegree=constrainingDegreeOfValue(searchCSP.domain[nxt][i]);
	}
	sort(domainOrder, domainOrder+sz, cmpCSPValues);
	bool res;
	for(int i=0; i<sz; i++){
		isAssigned[nxt]=true;
		sol[nxt]=domainOrder.value;
		res=search(assignedNodes+1);
		if(res) return true;
		isAssigned[nxt]=false;
	}
	return false;
}

bool isFrequent(Subgraphs sub, CSP csp)
{
	marked.clear();
	searchSubgraph=sub;
	searchCSP=csp;
	
	for(int loops=0; loops<sub.numberOfNodes; loops++){
		node=findNextVariable(true);
		for(int i=0; i<(int)csp.domain[node].size(); i++){
			isAssigned[nxt]=true;
			sol[nxt]=csp.domain[node][i];
			search()
			isAssigned[nxt]=false;
		}
		
		domainTested[node]=true;
	}
	return true;
}
