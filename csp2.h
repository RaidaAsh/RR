#include "SubgraphDebugKit.h"

CSP findDomain(int edgeIndex)
{	CSP newCSP;
	newCSP.domain.resize(2);
	GraphEdge currEdge = distinctEdges[edgeIndex];
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
}

void restoreDomains(int nxt){
	pii stkTop;
	int sz = searchSubgraph.incomingAdjNode[nxt].size() + searchSubgraph.adjNode[nxt].size();
	for(int i = 0;i < sz;i++)
	{
		stkTop = stk.top();
		stk.pop();
		domainSize[stkTop.first] = stkTop.second;
	}
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
	assert(domainSizeConsistent(searchSubgraph));
	if(assignedNodes==searchSubgraph.numberOfNodes){
		 return true; //All variables have had valid assignments
	}
	int nxt=findNextVariable(false); //Next node to assign value to
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
		markNodes[sol[nxt]] = true;
		enforceArcConsistency(nxt,domainOrder[i].value);
		assert(domainSizeConsistent(searchSubgraph));
		res=search(assignedNodes+1);
		restoreDomains(nxt);
		markNodes[sol[nxt]] = false;
		isAssigned[nxt]=false;
		if(res) return true;
	}
	return false;
}

bool isFrequent(Subgraphs sub, CSP &csp)
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
		count=0;
		vector <int> pos;
		for(int i=0; i< (int)searchCSP.domain[nxt].size(); i++){
			if(marked.find(pii(nxt, searchCSP.domain[nxt][i]))!=marked.end()){
				count++;
				if(count==(int)threshold) break;
				continue;
			}
			isAssigned[nxt]=true;
			sol[nxt]=searchCSP.domain[nxt][i];
			markNodes[searchCSP.domain[nxt][i]] = true;
			assert(domainSizeConsistent(searchSubgraph));
			enforceArcConsistency(nxt,searchCSP.domain[nxt][i]);
			assert(domainSizeConsistent(searchSubgraph));
			if(search(1)){
				count++;
				for(int j=0; j<searchSubgraph.numberOfNodes; j++){
					marked.insert(pii(j, sol[j]));
				}
			}
			else{
				pos.push_back(i);
			}
			restoreDomains(nxt);
			markNodes[sol[nxt]] = false;
			isAssigned[nxt]=false;
			assert(domainSizeConsistent(searchSubgraph));
			if(count==(int)threshold) break;
		}
		
		for(int i = pos.size() - 1;i>=0;i--)searchCSP.domain[nxt].erase(searchCSP.domain[nxt].begin() + pos[i]);
		domainTested[nxt]=true;
		searchCSP.mnSize = min((unsigned int)searchCSP.domain[nxt].size(),searchCSP.mnSize);
		domainSize[nxt] = searchCSP.domain[nxt].size();
		if(count<(int)threshold){
			csp = searchCSP;
			return false;
		}
	}
	
	csp = searchCSP;
	return true;
}

