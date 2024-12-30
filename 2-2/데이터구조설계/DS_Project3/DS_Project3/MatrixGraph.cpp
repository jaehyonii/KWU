#include "MatrixGraph.h"
#include <iostream>
#include <vector>
#include <string>

MatrixGraph::MatrixGraph(bool type, int size) : Graph(type, size)
{
	m_Mat = new int* [size + 1];
	
	for (int i = 0; i < size + 1; i++)
		m_Mat[i] = new int[size + 1];
	for (int i = 1; i < size + 1; i++)
		for (int j = 1; j < size + 1; j++)
			m_Mat[i][j] = 0;
}

MatrixGraph::~MatrixGraph()
{
	for (int i = 0; i < m_Size + 1; i++)
		delete[] m_Mat[i];
	delete[] m_Mat;
}


void MatrixGraph::getAdjacentEdges(int vertex, map<int, int>* m, char option)
{
	if (option == 'Y')
		getAdjacentEdgesDirect(vertex, m);
	else
		getAdjacentEdgesUndirect(vertex, m);

}

void MatrixGraph::getAdjacentEdgesDirect(int vertex, map<int, int>* m)
{
	m->clear();

	for (int w = 1; w <= m_Size; w++) {
		if(m_Mat[vertex][w] != 0 )
			m->insert({ w, m_Mat[vertex][w] });
	}
}

void MatrixGraph::getAdjacentEdgesUndirect(int vertex, map<int, int>* m)
{
	m->clear();

	for (int w = 1; w <= m_Size; w++) {
		if (m_Mat[vertex][w] != 0)
			m->insert({ w, m_Mat[vertex][w] });
	}
	for (int h = 1; h <= m_Size; h++) {
		if (m_Mat[h][vertex] != 0)
			m->insert({ h, m_Mat[h][vertex] });
	}
}

void MatrixGraph::getIncomingEdges(int vertex, map<int, int>* m, char option)
{
	m->clear();

	if (option == 'Y') {
		for (int i = 1; i <= m_Size; i++)
			if (m_Mat[i][vertex] != 0)
				m->insert({ i ,m_Mat[i][vertex] });
	}
	else {
		getAdjacentEdgesUndirect(vertex, m);
	}
}

void MatrixGraph::insertEdge(int from, int to, int weight)	
{
	m_Mat[from][to] = weight;
}

bool MatrixGraph::printGraph(ofstream *fout)	
{
	if (m_Mat == NULL)
		return false;

	*fout << "======== PRINT========" << endl;
	*fout << "    ";
	for (int w = 1; w <= m_Size; w++)
		*fout << '[' << w << "] ";
	*fout << endl;
	for (int h = 1; h <= m_Size; h++) {
		*fout << '[' << h << ']';
		for (int w = 1; w <= m_Size; w++) {
			fout->width(3);
			*fout << m_Mat[h][w] << ' ';
		}
		*fout << endl;
	}
	*fout << "=====================" << endl << endl;
	return true;
}

vector<int>* MatrixGraph::getKwGraph()
{
	return NULL;
}