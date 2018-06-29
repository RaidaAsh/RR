#include "csp.h"


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


void subgraphExtension(Subgraphs currSubgraph, CSP csp){
	//globalVariable++;
	//if((globalVariable%10)==0) printf("%d\n", globalVariable);
	/*if(currSubgraph.numberOfEdges==1)*/ assert(incomingEdgesConsistent(currSubgraph) && edgesConsistent(currSubgraph) && domainConsistent(csp));
	//else assert(incomingEdgesConsistent(currSubgraph) && edgesConsistent(currSubgraph));
	printSubgraph(currSubgraph);
	printCSP(csp);
	/*if(userWantsToTerminate())
	{
		printf("Terminated\n");
		return;
	}*/
	if(!isFrequent(currSubgraph,csp)){
		printf("Infrequent\n");
		return;
	}
	else{
		printf("Frequent : %d\n",currSubgraph.numberOfNodes);
		freqSubgraphs.push_back(currSubgraph);
	}
	//char ch = getchar();
	//ch = getchar();
	//if(ch){}
	//else{}
	/*if(randomTermination()){
		return;
	}*/
	
	
	
	
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
		printf("New Edge: %d %d %d\n", distinctEdges[leastEdgeRemaining].u, distinctEdges[leastEdgeRemaining].v, distinctEdges[leastEdgeRemaining].edgeLabel); 
		printCSP(csp);
		subgraphExtension(newSubgraph,csp);
		printf("Done with this edge\n");
	}
}
