#include "takeInput.h"

/**************************************************************************************************/
inline void findLabelRelations(){
	labelRelations.clear();
	ppi newp;
	for(int i=0; i<numberOfEdges; i++){
		newp=ppi(pii(nodeLabels[edgeList[i].u], nodeLabels[edgeList[i].v]), edgeList[i].edgeLabel);
		labelRelations[newp]=edgeList[i].w;
	}
}

inline void printLabelRelations(){
	ppi temp;
	for(auto it=labelRelations.begin(); it!=labelRelations.end(); it++){
		temp=it->first;
		cout<<temp.first.first<<" "<<temp.first.second<<" "<<temp.second<<" "<<(it->second)<<endl; 
	}
}
