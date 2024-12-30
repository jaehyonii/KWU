#ifndef _LIST_H_
#define _LIST_H_

#include "Graph.h"

class ListGraph : public Graph{	
private:
	map < int, int >* m_List;

	vector<int>* kw_graph;
	
public:	
	ListGraph(bool type, int size);
	~ListGraph();
		
	void getAdjacentEdges(int vertex, map<int, int>* m, char option);	
	void getAdjacentEdgesDirect(int vertex, map<int, int>* m);
	void getAdjacentEdgesUndirect(int vertex, map<int, int>* m);
	void getIncomingEdges(int vertex, map<int, int>* m, char option);
	void insertEdge(int from, int to, int weight);	
	bool printGraph(ofstream *fout);
	vector<int>* getKwGraph();
};

#endif