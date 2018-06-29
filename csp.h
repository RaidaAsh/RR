#include "SubgraphDebugKit.h"

CSP findDomain(int edgeIndex)
{
	printf("Finding domain:\n");
	CSP newCSP;
	newCSP.domain.resize(2);
	GraphEdge currEdge = distinctEdges[edgeIndex];
	int labelu=nodeLabels[currEdge.u];
	int labelv=nodeLabels[currEdge.v];
	int sz = nodesWithLabel[labelu].size();
	int currNode;
	printf("For domain of %d\n", labelu);
	for(int i = 0;i < sz; i++)
	{
		currNode = nodesWithLabel[labelu][i];
		printf("Considering node %d\n", currNode);
		if(outgoingLabels[currNode].find(pii(nodeLabels[currEdge.v],currEdge.edgeLabel)) != outgoingLabels[currNode].end())
		{
			printf("pushing %d in domain\n", currNode);
			newCSP.domain[0].push_back(currNode);
		}
	}
	sz = nodesWithLabel[labelv].size();
	printf("For domain of %d\n", labelv);
	for(int i = 0;i < sz; i++)
	{
		currNode = nodesWithLabel[labelv][i];
		printf("Considering node %d\n", currNode);
		if(incomingLabels[currNode].find(pii(nodeLabels[currEdge.u],currEdge.edgeLabel)) != incomingLabels[currNode].end())
		{
			printf("pushing %d in domain\n", currNode);
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
int domainSize[NODES];
Subgraphs searchSubgraph;
CSPValues domainOrder[NODES];
stack <pii> stk;
bool markNodes[NODES];

bool domainSizeConsistent(Subgraphs sub){
	for(int i=0; i<(int)sub.numberOfNodes; i++){
		if(domainSize[i]<0 || domainSize[i]>=numberOfNodes) return false;
	}
	int val;
	for(int i=0; i<(int)searchCSP.domain.size(); i++){
		for(int j=0; j<(int)domainSize[i]; j++){
			val=searchCSP.domain[i][j];
			if(val<0 || val>=numberOfNodes) return false;
		}
	}
	return true;
}


bool cmpCSPValues(CSPValues a, CSPValues b){
	if(markNodes[a.value])return false;
	if(markNodes[b.value])return true;
	return a.constrainingDegree<b.constrainingDegree;
}

void enforceArcConsistency(int var, int val){
	int v, forwardIterator;
	for(int i=0; i<(int)searchSubgraph.adjNode[var].size(); i++){
		v=searchSubgraph.adjNode[var][i];
		forwardIterator=-1;
		for(int j=0; j<domainSize[v]; j++){
			if(incomingNodes[searchCSP.domain[v][j]].find(pii(val, searchSubgraph.adjLabel[var][i]))==incomingNodes[searchCSP.domain[v][j]].end()){
				if(forwardIterator==-1){
					forwardIterator=j;
				}
			}
			else{
				if(forwardIterator!=-1){
					swap(searchCSP.domain[v][j], searchCSP.domain[v][forwardIterator]);
					forwardIterator++;
				}
			}
		}
		
		stk.push(pii(v, domainSize[v]));
		if(forwardIterator!=-1) domainSize[v]=forwardIterator;
	}
	for(int i=0; i<(int)searchSubgraph.incomingAdjNode[var].size(); i++){
		v=searchSubgraph.incomingAdjNode[var][i];
		forwardIterator=-1;
		for(int j=0; j<domainSize[v]; j++){
			if(outgoingNodes[searchCSP.domain[v][j]].find(pii(val, searchSubgraph.incomingAdjLabel[var][i]))==outgoingNodes[searchCSP.domain[v][j]].end()){
				if(forwardIterator==-1){
					forwardIterator=j;
				}
			}
			else{
				if(forwardIterator!=-1){
					swap(searchCSP.domain[v][j], searchCSP.domain[v][forwardIterator]);
					forwardIterator++;
				}
			}
		}
		stk.push(pii(v, domainSize[v]));
		if(forwardIterator!=-1) domainSize[v]=forwardIterator;
	}
	/*printf("After enforcement\n");
	for(int i = 0 ;i < searchSubgraph.numberOfNodes;i++)
		{
			printf("%d: %d\n\n",i,domainSize[i]);
			for(int j =0;j<domainSize[i];j++)
			{
				printf("%d ",searchCSP.domain[i][j]);
			}
			printf("\n");
		}*/
		
}

void restoreDomains(int nxt){
	pii stkTop;
	int sz = searchSubgraph.incomingAdjNode[nxt].size() + searchSubgraph.adjNode[nxt].size();
	for(int i = 0;i < sz;i++)
	{
		stkTop = stk.top();
		stk.pop();
		domainSize[stkTop.first] = stkTop.second;
	}/*
	printf("After restoration \n");
	for(int i = 0 ;i < searchSubgraph.numberOfNodes;i++)
	printf("%d\n\n",domainSize[i]);*/
}

int constrainingDegreeOfValue(int nxt,int value){ //Number of additional constraints imposed by assigining value to node
	int count=0;
	for(int i=0; i<(int)searchCSP.domain.size(); i++){
		for(int j=0; j < domainSize[i]; j++){
			if(searchCSP.domain[i][j]==value){
				count++;
				break;
			}
		}
	}
	int v;
	for(int i=0; i<(int)searchSubgraph.adjNode[nxt].size(); i++){
		v=searchSubgraph.adjNode[nxt][i];
		for(int j=0; j < domainSize[v]; j++){
			if(outgoingNodes[searchCSP.domain[v][j]].find(pii(value, searchSubgraph.adjLabel[nxt][i]))==outgoingNodes[searchCSP.domain[v][j]].end())
				count++;
		}
	}
	
	for(int i=0; i<(int)searchSubgraph.incomingAdjNode[nxt].size(); i++){
		v=searchSubgraph.incomingAdjNode[nxt][i];
		for(int j=0; j<(int)domainSize[v]; j++){
			if(incomingNodes[searchCSP.domain[v][j]].find(pii(value, searchSubgraph.incomingAdjLabel[nxt][i]))==incomingNodes[searchCSP.domain[v][j]].end())
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
			else if(domainSize[i] < domainSize[mn]){
				mn=i; //Most constrained variable
			}
			else if(domainSize[i] == domainSize[mn] && (searchSubgraph.adjNode[i].size()+searchSubgraph.incomingAdjNode[i].size())>(searchSubgraph.adjNode[mn].size()+searchSubgraph.incomingAdjNode[mn].size())){
				mn=i; //Most constraining value
			}
		}
	}
	return mn;	
}



bool search(int assignedNodes){
	//cout<<"Assigned Nodes="<<assignedNodes<<endl;
	assert(domainSizeConsistent(searchSubgraph));
	if(assignedNodes==searchSubgraph.numberOfNodes){
		 //cout<<"Returning true\n";
		 return true; //All variables have had valid assignments
	}
	int nxt=findNextVariable(false); //Next node to assign value to
	//printf("Next variable=%d\n", nxt);
	if(domainSize[nxt]==0) return false;
	int sz=domainSize[nxt];
	for(int i=0; i<sz; i++){
		domainOrder[i].value=searchCSP.domain[nxt][i];
		domainOrder[i].constrainingDegree=constrainingDegreeOfValue(nxt, searchCSP.domain[nxt][i]);
	}
	sort(domainOrder, domainOrder+sz, cmpCSPValues);
	bool res;
	for(int i=0; i<sz; i++){
		if(markNodes[domainOrder[i].value])break;
		isAssigned[nxt]=true;
		sol[nxt]=domainOrder[i].value;
		printf("Assigning %d to %d\n", sol[nxt], nxt);
		markNodes[sol[nxt]] = true;
		enforceArcConsistency(nxt,domainOrder[i].value);
		assert(domainSizeConsistent(searchSubgraph));
		res=search(assignedNodes+1);
		restoreDomains(nxt);
		markNodes[sol[nxt]] = false;
		isAssigned[nxt]=false;
		//printf("Domains restored\n");
		if(res) return true;
	}
	return false;
}

bool isFrequent(Subgraphs sub, CSP csp)
{
	marked.clear();
	searchSubgraph=sub;
	searchCSP=csp;
	for(int i=0; i<sub.numberOfNodes; i++){
		domainSize[i]=searchCSP.domain[i].size();
		domainTested[i] = false;
	}
	while(!stk.empty()) stk.pop();
	int count, nxt;
	for(int loops=0; loops<sub.numberOfNodes; loops++){
		nxt=findNextVariable(true);
		//cout<<"Testing for "<<nxt<<endl;
		count=0;
		for(int i=0; i<(int)searchCSP.domain[nxt].size(); i++){
			printf("Considering assignment of %d to %d\n", searchCSP.domain[nxt][i], nxt);
			if(markNodes[searchCSP.domain[nxt][i]])
			{
				assert(false);
				printf("Breaking\n");
				break;
			}
			printf("Assigning it\n");
			isAssigned[nxt]=true;
			sol[nxt]=searchCSP.domain[nxt][i];
			markNodes[searchCSP.domain[nxt][i]] = true;
			printf("Assigning %d to %d\n", sol[nxt], nxt);
			assert(domainSizeConsistent(searchSubgraph));
			enforceArcConsistency(nxt,searchCSP.domain[nxt][i]);
			assert(domainSizeConsistent(searchSubgraph));
			//puts("Arc consistency enforced\n");
			if(search(1)) count++;
			restoreDomains(nxt);
			markNodes[sol[nxt]] = false;
			//printf("Domains restored\n");
			isAssigned[nxt]=false;
			assert(domainSizeConsistent(searchSubgraph));
			if(count==(int)threshold) break;
		}
		printf("Count of %d: %d\n\n",nxt,count);
		if(count<(int)threshold) return false;
		domainTested[nxt]=true;
	}
	return true;
}
