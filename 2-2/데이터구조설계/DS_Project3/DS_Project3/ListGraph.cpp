#include "ListGraph.h"
#include <iostream>
#include <utility>

ListGraph::ListGraph(bool type, int size) : Graph(type, size)
{
	m_List = new map < int, int >[size + 1];

	kw_graph = new vector<int>[size + 1];
}

ListGraph::~ListGraph()	
{
	delete[] m_List;
	delete[] kw_graph;
}

void ListGraph::getAdjacentEdges(int vertex, map<int, int>* m, char option)
{
	if (option == 'Y')
		getAdjacentEdgesDirect(vertex, m);
	else
		getAdjacentEdgesUndirect(vertex, m);

}

void ListGraph::getAdjacentEdgesDirect(int vertex, map<int, int>* m)	// Definition of getAdjacentEdges(Directed graph)
{
	m->clear();
	
	*m = m_List[vertex];
}

void ListGraph::getAdjacentEdgesUndirect(int vertex, map<int, int>* m)	 // Definition of getAdjacentEdges(No Direction == Undirected)
{
	m->clear();

	for (int i = 1; i <= m_Size; i++) {
		auto it = m_List[i].find(vertex);
		if (it != m_List[i].end())
			m->insert({ i, it->second });
	}
	for (auto it : m_List[vertex]) {
		if (it.second != 0)
			m->insert(it);
	}
}

void ListGraph::getIncomingEdges(int vertex, map<int, int>* m, char option)
{
	m->clear();

	if (option == 'Y') {
		for (int i = 1; i <= m_Size; i++) {
			auto it =  m_List[i].find(vertex);
			if (it != m_List[i].end())
				m->insert({i, it->second});
		}
	}
	else {
		getAdjacentEdgesUndirect(vertex, m);
	}
}

void ListGraph::insertEdge(int from, int to, int weight) //Definition of insertEdge
{
	m_List[from].insert({ to, weight });

	kw_graph[from].push_back(to);
	kw_graph[to].push_back(from);
}

bool ListGraph::printGraph(ofstream *fout)	//Definition of print Graph
{
	if (m_List == NULL)
		return false;

	*fout << "======== PRINT========" << endl;
	for (int i = 1; i <= m_Size; i++) {
		*fout << '[' << i << ']';
		for (auto it : m_List[i])
			*fout << "-> (" << it.first << ',' << it.second << ')';
		*fout << endl;
	}
	*fout << "=====================" << endl << endl;
	return true;
}

vector<int>* ListGraph::getKwGraph()
{
	return kw_graph;
}
