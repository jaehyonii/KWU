#include "Graph.h"

Graph::Graph(bool type, int size)
{
	m_Type = type; // L: 0, M: 1
	m_Size = size;
}

Graph::~Graph()	
{
	
}


bool Graph::getType(){return m_Type;}	
int Graph::getSize(){return m_Size;}