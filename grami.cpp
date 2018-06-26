#include "sort.h"

/**********************************************************************/


inline bool cmpEdges(GraphEdge a, GraphEdge b){
	if(nodeLabels[a.u]!=nodeLabels[b.u]) return nodeLabelList[nodeLabels[a.u]].indexInSortedList<nodeLabelList[nodeLabels[b.u]].indexInSortedList;
	if(nodeLabels[a.v]!=nodeLabels[b.v]) return nodeLabelList[nodeLabels[a.v]].indexInSortedList<nodeLabelList[nodeLabels[b.v]].indexInSortedList;
	if(a.edgeLabel!=b.edgeLabel) return edgeLabelList[a.edgeLabel].indexInSortedList<edgeLabelList[b.edgeLabel].indexInSortedList;
	return false;
}

inline void processEdgeList(){
	sort(edgeList, edgeList+numberOfEdges, cmpEdges);
	for(int i=0; i<numberOfEdges; i++){
		adj[edgeList[i].u].push_back(i);
	}
	memset(startIterator, 0, sizeof(startIterator));
}

void printEdgeList(){
	for(int i=0; i<numberOfEdges; i++){
		printf("%d %d %f %d\n", edgeList[i].u, edgeList[i].v, edgeList[i].w, edgeList[i].edgeLabel);
	}
}

inline bool hasSameLabels(GraphEdge a, GraphEdge b){
	return (nodeLabels[a.u]==nodeLabels[b.u] && nodeLabels[a.v]==nodeLabels[b.v] && a.edgeLabel==b.edgeLabel);
}

inline void insertDistinctEdge(GraphEdge a){
	distinctEdges[numberOfDistinctEdges]=a;
	numberOfDistinctEdges++;
}

inline void findDistinctEdges(){ //Assumes edges in EdgeList are sorted in canonical order
	numberOfDistinctEdges=0;
	insertDistinctEdge(edgeList[0]);
	for(int i=1; i<numberOfEdges; i++){
		if(!hasSameLabels(edgeList[i-1], edgeList[i])){
			insertDistinctEdge(edgeList[i]);
		}
	}
}

inline void printDistinctEdges(){
	puts("");
	for(int i=0; i<numberOfDistinctEdges; i++){
		printf("%d %d %d\n", nodeLabels[distinctEdges[i].u], nodeLabels[distinctEdges[i].v], distinctEdges[i].edgeLabel); 
	}
	puts("");
}


Subgraphs extendBackwardEdge(Subgraphs currSubgraph, int u, int v, int edgeLabel){
	assert(v<u);
	Subgraphs newSubgraph;
	newSubgraph=currSubgraph;
	newSubgraph.numberOfEdges++;
	newSubgraph.adjNode[u].push_back(v);
	newSubgraph.adjLabel[u].push_back(edgeLabel);
	newSubgraph.incomingAdjNode[v].push_back(u);
	newSubgraph.incomingAdjLabel[v].push_back(edgeLabel);
	return newSubgraph;
}

CSP domainAfterBackwardExtension(int u, int uLabel, int v, int vLabel, int edgeLabel, CSP csp){
	CSP newCsp=csp;
	int sz=newCsp.domain[u].size();
	int forwardIterator=-1, val;
	for(int i=0; i<sz; i++){
		val=newCsp.domain[u][i];
		if(outgoingLabels[val].find(pii(vLabel, edgeLabel))==outgoingLabels[val].end()){ //Need to remove val from domain[u]
			if(forwardIterator==-1){
				forwardIterator=i;
			}
		}
		else{ //Keep val in domain[u]
			if(forwardIterator==-1) continue;
			swap(newCsp.domain[u][i], newCsp.domain[u][forwardIterator]);
			forwardIterator++;
		}
	}
	for(int i=(forwardIterator==-1) ? sz: forwardIterator; i<sz; i++){
		newCsp.domain[u].pop_back();
	}
	newCsp.mnSize=min(newCsp.mnSize, (unsigned int) newCsp.domain[u].size());
	sz=newCsp.domain[v].size();
	forwardIterator=-1;
	for(int i=0; i<sz; i++){
		val=newCsp.domain[v][i];
		if(incomingLabels[val].find(pii(uLabel, edgeLabel))==incomingLabels[val].end()){ //Need to remove val from domain[u]
			if(forwardIterator==-1){
				forwardIterator=i;
			}
		}
		else{ //Keep val in domain[u]
			if(forwardIterator==-1) continue;
			swap(newCsp.domain[v][i], newCsp.domain[v][forwardIterator]);
			forwardIterator++;
		}
	}
	for(int i=(forwardIterator==-1) ? sz: forwardIterator; i<sz; i++){
		newCsp.domain[v].pop_back();
	}
	newCsp.mnSize=min(newCsp.mnSize, (unsigned int) newCsp.domain[v].size());
	return newCsp;
	return newCsp;
	
}

CSP domainAfterForwardExtension(int u, int uLabel, int vLabel, int edgeLabel, CSP csp){
	CSP newCsp=csp;
	vector <int> domainOfExtendedNode;
	int nodeInConsideration;
	int sz=nodesWithLabel[vLabel].size();
	for(int i=0; i<sz; i++){
		nodeInConsideration=nodesWithLabel[vLabel][i];
		if(incomingLabels[nodeInConsideration].find(pii(uLabel, edgeLabel))!=incomingLabels[nodeInConsideration].end()){
			domainOfExtendedNode.push_back(nodeInConsideration);
		}
	}
	newCsp.domain.push_back(domainOfExtendedNode);
	newCsp.mnSize=min(newCsp.mnSize,(unsigned int) domainOfExtendedNode.size());
	domainOfExtendedNode.clear();
	
	sz=newCsp.domain[u].size();
	int forwardIterator=-1, val;
	for(int i=0; i<sz; i++){
		val=newCsp.domain[u][i];
		if(outgoingLabels[val].find(pii(vLabel, edgeLabel))==outgoingLabels[val].end()){ //Need to remove val from domain[u]
			if(forwardIterator==-1){
				forwardIterator=i;
			}
		}
		else{ //Keep val in domain[u]
			if(forwardIterator==-1) continue;
			swap(newCsp.domain[u][i], newCsp.domain[u][forwardIterator]);
			forwardIterator++;
		}
	}
	for(int i=(forwardIterator==-1) ? sz: forwardIterator; i<sz; i++){
		newCsp.domain[u].pop_back();
	}
	newCsp.mnSize=min(newCsp.mnSize, (unsigned int) newCsp.domain[u].size());
	return newCsp;
}

Subgraphs extendForwardEdge(Subgraphs currSubgraph, int u, int v, int vLabel, int edgeLabel){
	assert(v>u);
	Subgraphs newSubgraph;
	vector <int> newVec;
	newSubgraph=currSubgraph;
	newSubgraph.numberOfNodes++;
	newSubgraph.numberOfEdges++;
	newSubgraph.adjNode[u].push_back(v);
	newSubgraph.adjLabel[u].push_back(edgeLabel);
	newSubgraph.adjNode.push_back(newVec);
	newSubgraph.adjLabel.push_back(newVec);
	
	
	newSubgraph.incomingAdjNode.push_back(newVec);
	newSubgraph.incomingAdjLabel.push_back(newVec);
	newSubgraph.incomingAdjNode[v].push_back(u);
	newSubgraph.incomingAdjLabel[v].push_back(edgeLabel);
	
	newSubgraph.nodeLabels.push_back(vLabel);
	newSubgraph.rightmostPath.push(v);
	return newSubgraph;
}

void printCSP(CSP csp){
	printf("CSP:\n");
	printf("Min Size=%d\n", csp.mnSize);
	for(int i=0; i<(int)csp.domain.size(); i++){
		printf("%d: ", i); 
		for(int j=0; j<(int)csp.domain[i].size(); j++){
			printf("%d ", csp.domain[i][j]);
		}
		puts("");
	}
	puts("");
	puts("");
	puts("");
}

void printSubgraph(Subgraphs currSubgraph)
{
	printf("Subgraph:\n");
	//getchar();
	printf("\nNodes in subgraph: %d\n Edges in Subgraph: %d\n",currSubgraph.numberOfNodes, currSubgraph.numberOfEdges);
	printf("\nThe edges in the subgraph are\n"); 
	for(int i = 0;i < currSubgraph.numberOfNodes;i++)
	{
		for(unsigned int j = 0;j < currSubgraph.adjNode[i].size();j++)
		{
			printf("%d %d %d\n",i,currSubgraph.adjNode[i][j],currSubgraph.adjLabel[i][j]);
		}
	}
	printf("Labels of the nodes: ");
	for(unsigned int i = 0;i< currSubgraph.nodeLabels.size();i++)
	{
		printf("%d ",currSubgraph.nodeLabels[i]);
	}
	puts("");
	if(currSubgraph.rightmostPath.empty())printf("Empty\n");
	else printf("Stack top : %d\n",currSubgraph.rightmostPath.top());
}
int globalVariable = 0;

inline bool userWantsToTerminate(){
	char terminator[5];
	printf("Do you want to terminate?\n");
	cin>>terminator;
	return (terminator[0] == 'y');
}

inline bool randomTermination(){
	return (rand()%100)<80;
}

bool inAdjEdges(Subgraphs sub, int u, int v, int elabel){
	for(int i=0; i<(int)sub.adjNode[u].size(); i++){
		if(sub.adjNode[u][i]==v && sub.adjLabel[u][i]==elabel) return true;
	}
	return false;
}

bool inInEdges(Subgraphs sub, int u, int v, int elabel){
	for(int i=0; i<(int)sub.incomingAdjNode[u].size(); i++){
		if(sub.incomingAdjNode[u][i]==v && sub.incomingAdjLabel[u][i]==elabel) return true;
	}
	return false;
}

bool incomingEdgesConsistent(Subgraphs sub){
	for(int i=0; i<sub.numberOfNodes; i++){
		for(int j=0; j<(int)sub.incomingAdjNode[i].size(); j++){
			if(!inAdjEdges(sub, sub.incomingAdjNode[i][j], i, sub.incomingAdjLabel[i][j])){
				return false;
			}
		}
	}
	return true;
}

bool edgesConsistent(Subgraphs sub){
	for(int i=0; i<sub.numberOfNodes; i++){
		for(int j=0; j<(int)sub.adjNode[i].size(); j++){
			if(!inInEdges(sub, sub.adjNode[i][j], i, sub.adjLabel[i][j])){
				return false;
			}
		}
	}
	return true;
}
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
void subgraphExtension(Subgraphs currSubgraph, CSP csp){
	globalVariable++;
	if((globalVariable%10)==0) printf("%d\n", globalVariable);
	//assert(incomingEdgesConsistent(currSubgraph) && edgesConsistent(currSubgraph));
	printSubgraph(currSubgraph);
	printCSP(csp);	
	if(isFrequent(currSubgraph,csp)){
			printf("Finished checking frequency\n");
	}
	if(userWantsToTerminate())
	{
		printf("Terminated\n");
		return;
	}
	
	/*if(randomTermination()){
		return;
	}*/
	
	if(csp.mnSize<threshold){
		//printf("Terminating anyways\n");
		return;
	}
	
	
	Subgraphs newSubgraph;
	int currNode;
	int low, sz;
	CSP newCsp;
	while(!currSubgraph.rightmostPath.empty()){
		currNode=currSubgraph.rightmostPath.top();
		sz=currSubgraph.adjNode[currNode].size();
		low=(sz==0) ? 0 : currSubgraph.adjNode[currNode][sz-1]+1;
		for(int i=low; i<currNode; i++){
			for(int j=0; j<numberOfEdgeLabels; j++){
				if(labelRelations.find(ppi(pii(currSubgraph.nodeLabels[currNode], currSubgraph.nodeLabels[i]), j))==labelRelations.end()) continue;
				newSubgraph=extendBackwardEdge(currSubgraph, currNode, i, j);
				newCsp = domainAfterBackwardExtension(currNode,currSubgraph.nodeLabels[currNode],i,currSubgraph.nodeLabels[i],j,csp);
				subgraphExtension(newSubgraph,newCsp);
			}
		}
		for(int i=0; i<numberOfNodeLabels; i++){
			for(int j=0; j<numberOfEdgeLabels; j++){
				if(labelRelations.find(ppi(pii(currSubgraph.nodeLabels[currNode], i), j))==labelRelations.end()) continue;
				newSubgraph=extendForwardEdge(currSubgraph, currNode, currSubgraph.numberOfNodes, i, j);
				newCsp=domainAfterForwardExtension(currNode, currSubgraph.nodeLabels[currNode], i, j, csp);
				subgraphExtension(newSubgraph,newCsp);
			}
		}
		currSubgraph.rightmostPath.pop();
	}
}

void clearSubgraph(Subgraphs &sub){
	sub.incomingAdjNode.clear();
	sub.incomingAdjLabel.clear();
	sub.adjNode.clear();
	sub.adjLabel.clear();
	sub.nodeLabels.clear();
	while(!sub.rightmostPath.empty()) sub.rightmostPath.pop();
}

void initializeSingleEdgeGraph(GraphEdge e, Subgraphs &sub){
	sub.numberOfNodes = 2;
	sub.numberOfEdges = 1;
	sub.incomingAdjNode.resize(2);
	sub.incomingAdjLabel.resize(2);
	sub.incomingAdjNode[1].push_back(0);
	sub.incomingAdjLabel[1].push_back(e.edgeLabel);
	sub.adjNode.resize(2);
	sub.adjLabel.resize(2);
	sub.adjNode[0].push_back(1);
	sub.adjLabel[0].push_back(e.edgeLabel);
	sub.rightmostPath.push(0);
	sub.rightmostPath.push(1);
	sub.nodeLabels.push_back(nodeLabels[e.u]);
	sub.nodeLabels.push_back(nodeLabels[e.v]);
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
void gSpanInit()
{
	Subgraphs newSubgraph;
	leastEdgeRemaining = 0;
	CSP csp;
	for(;leastEdgeRemaining < numberOfDistinctEdges;leastEdgeRemaining++)
	{
		clearSubgraph(newSubgraph);		
		initializeSingleEdgeGraph(distinctEdges[leastEdgeRemaining], newSubgraph);
		csp=findDomain(leastEdgeRemaining);
		subgraphExtension(newSubgraph,csp);
		printf("Done with this edge\n");
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




int main()
{
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
