#include "structures.h"

/**************** GLOBAL VARIABLES *********************************/

int numberOfNodes,numberOfEdges,numberOfNodeLabels,numberOfEdgeLabels, numberOfDistinctEdges;
vector <int> adj[NODES]; // adjacency list
int startIterator[NODES];
double threshold;
int leastEdgeRemaining = 0;
vector <Subgraphs> freqSubgraphs;
set <pii> outgoingLabels[NODES],incomingLabels[NODES], outgoingNodes[NODES], incomingNodes[NODES];
map < ppi , double > labelRelations;
int freq[NODES + EDGES],taken[NODES + EDGES];
