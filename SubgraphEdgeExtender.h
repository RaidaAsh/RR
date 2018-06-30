#include "DistinctLabelledEdgeFinder.h"

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

