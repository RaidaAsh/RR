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
long long int toBeDeleted[NODES],iterationNo;

bool domainSizeConsistent(Subgraphs sub){
	for(int i=0; i<(int)sub.numberOfNodes; i++){
		if(domainSize[i]<0 || domainSize[i]>numberOfNodes){
			//printf("Domain Size of %d is %d\n", i, domainSize[i]);
			return false;
		}
	}
	int val;
	for(int i=0; i<(int)searchCSP.domain.size(); i++){
		for(int j=0; j<(int)min(domainSize[i], (int)searchCSP.domain[i].size()); j++){
			val=searchCSP.domain[i][j];
			if(val<0 || val>=numberOfNodes){
				//printf("Domain of %d contains %d\n", i, val);
				return false;
			}
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
	marked.clear(); //No assignments are marked initially
	
	searchSubgraph=sub;
	searchCSP=csp;  //Saving the subgraph and csp in global data structures for fast access
	
	for(int i=0; i<sub.numberOfNodes; i++){
		domainSize[i]=searchCSP.domain[i].size(); //Initialize the valid domain size of each node
		domainTested[i] = false;  //Initially no node of the subgraph has had its domains checked
	}
	while(!stk.empty()) stk.pop();
	
	int count, nxt;
	int forwardIterator, sz;
	
	
	for(int loops=0; loops<sub.numberOfNodes; loops++){ //Loop over all nodes
		nxt=findNextVariable(true); //Find variable to test domain
		
		//DEBUG STARTS
		printf("Testing %d's domain\n", nxt);
		//DEBUG ENDS
		
		count=0; //Tracks number of valid assignments for nxt
		
		iterationNo++;
		sz =  searchCSP.domain[nxt].size();
		
		for(int i=0; i<sz; i++){ //Loop over all domain elements of nxt
			
			//DEBUG STARTS
			printf("Considering assignment of %d to %d\n", i, nxt);
			//DEBUG ENDS
			
			if(marked.find(pii(nxt, searchCSP.domain[nxt][i]))!=marked.end()){  //If a valid solution satisfying this assignment
																				//has already been found, no need to search again
 				count++;
 				
				if(count==(int)threshold) break; //If count already reaches threshold, no need to search again
				continue;
			
			}
			
			isAssigned[nxt]=true;
			sol[nxt]=searchCSP.domain[nxt][i];
			markNodes[searchCSP.domain[nxt][i]] = true;
			enforceArcConsistency(nxt,searchCSP.domain[nxt][i]);
			
			if(search(1)){
				count++;
				
				//DEBUG STARTS
				puts("Match found:");
				//DEBUG ENDS
				
				for(int j=0; j<searchSubgraph.numberOfNodes; j++){
					
					//DEBUG STARTS
					printf("%d ", sol[j]);
					//DEBUG ENDS
					
					marked.insert(pii(j, sol[j]));
				}
				
				//DEBUG STARTS
				puts("");
				//DEBUG ENDS
				
			}
			else{
				toBeDeleted[sol[nxt]] = iterationNo;
			}
			restoreDomains(nxt);
			markNodes[sol[nxt]] = false;
			isAssigned[nxt]=false;
			assert(domainSizeConsistent(searchSubgraph));
			if(count==(int)threshold) break;
		}
		forwardIterator = -1;
		for(int i = 0;i < sz;i++){
			if(toBeDeleted[searchCSP.domain[nxt][i]] == iterationNo){
				if(forwardIterator == -1){
					forwardIterator = i;
				}
			}
			else{
				if(forwardIterator != -1){
					swap(searchCSP.domain[nxt][i],searchCSP.domain[nxt][forwardIterator]);
					forwardIterator++;
				}
			}
		}
		if(forwardIterator != -1)
		{
			for(int i = forwardIterator;i < sz;i++)
			{
				searchCSP.domain[nxt].pop_back();
			}
			searchCSP.mnSize = min((unsigned int)searchCSP.domain[nxt].size(),searchCSP.mnSize);
			domainSize[nxt] = searchCSP.domain[nxt].size();
		}
		
		if(count<(int)threshold){
			return false;
		}
		domainTested[nxt]=true;
	}
	csp = searchCSP;
	return true;
}
