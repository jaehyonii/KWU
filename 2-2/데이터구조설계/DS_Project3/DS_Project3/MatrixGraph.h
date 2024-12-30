#ifndef _MATRIX_H_
#define _MATRIX_H_
#include <map>
#include "Graph.h"

class MatrixGraph : public Graph{	
private:
	int** m_Mat;

public:
	MatrixGraph(bool type, int size);
	~MatrixGraph();
		
	void getAdjacentEdges(int vertex, map<int, int>* m, char option);	
	void getAdjacentEdgesDirect(int vertex, map<int, int>* m);
	void getAdjacentEdgesUndirect(int vertex, map<int, int>* m);
	void getIncomingEdges(int vertex, map<int, int>* m, char option);
	void insertEdge(int from, int to, int weight);	
	bool printGraph(ofstream *fout);
	vector<int>* getKwGraph();
};

#endif