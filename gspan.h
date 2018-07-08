#include "csp.h"


void subgraphExtension(Subgraphs currSubgraph, CSP csp){
	
	//DEBUG STARTS
	puts("Extended Subgraph");
	printSubgraph(currSubgraph);
	printCSP(csp);
	//DEBUG ENDS
	
	if(csp.mnSize<(unsigned int)threshold){
		
		//DEBUG STARTS
		printf("Subgraph's mininimum domain size is too small, so it is infrequent\n");
		//DEBUG ENDS
		
		return;
	}
	if(!isFrequent(currSubgraph,csp)){
		
		//DEBUG STARTS
		printf("Subgraph's deemed infrequent by csp\n");
		//DEBUG ENDS
		
		return;
	}
	else{
		
		//DEBUG STARTS
		printf("This subgraph is frequent\n");
		//DEBUG ENDS
		
		freqSubgraphs.push_back(currSubgraph);
	}
	
	Subgraphs newSubgraph;
	int currNode;
	int low, sz;
	CSP newCsp;
	
	while(!currSubgraph.rightmostPath.empty()){
		
		currNode=currSubgraph.rightmostPath.top(); //Node to be extended
		
		
		//Considering backward extensions from current Node
		sz=currSubgraph.adjNode[currNode].size();
		low=(sz==0) ? 0 : currSubgraph.adjNode[currNode][sz-1]+1; //low represents lowest non-adjacent node in the subgraph to which a backward edge may be drawn
		
		for(int i=low; i<currNode; i++){
			for(int j=0; j<numberOfEdgeLabels; j++){
				//if(labelRelations.find(ppi(pii(currSubgraph.nodeLabels[currNode], currSubgraph.nodeLabels[i]), j))==labelRelations.end()) continue; //If such an edge doesn't exist in a graph. continue
				newSubgraph=extendBackwardEdge(currSubgraph, currNode, i, j);
				newCsp = domainAfterBackwardExtension(currNode,currSubgraph.nodeLabels[currNode],i,currSubgraph.nodeLabels[i],j,csp);
				subgraphExtension(newSubgraph,newCsp);
			}
		}
		
		//Considering forward extensions from current Node
		for(int i=0; i<numberOfNodeLabels; i++){
			for(int j=0; j<numberOfEdgeLabels; j++){
				//if(labelRelations.find(ppi(pii(currSubgraph.nodeLabels[currNode], i), j))==labelRelations.end()) continue; //If such an edge doesn't exist in a graph. continue
				newSubgraph=extendForwardEdge(currSubgraph, currNode, currSubgraph.numberOfNodes, i, j);
				newCsp=domainAfterForwardExtension(currNode, currSubgraph.nodeLabels[currNode], i, j, csp);
				subgraphExtension(newSubgraph,newCsp);
			}
		}
		
		currSubgraph.rightmostPath.pop(); //All possible extensions from the current node have been made, popping it from the rightmost path
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
		//printf("New Edge: %d %d %d\n", distinctEdges[leastEdgeRemaining].u, distinctEdges[leastEdgeRemaining].v, distinctEdges[leastEdgeRemaining].edgeLabel);
		csp=findDomain(leastEdgeRemaining); 
		//printCSP(csp);
		subgraphExtension(newSubgraph,csp);
		//printf("Done with this edge\n");
		//labelRelations.erase(ppi(pii(nodeLabels[distinctEdges[leastEdgeRemaining].u], nodeLabels[distinctEdges[leastEdgeRemaining].v]), distinctEdges[leastEdgeRemaining].edgeLabel));
	}
}
