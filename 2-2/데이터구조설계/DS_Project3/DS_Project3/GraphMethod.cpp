#include <iostream>
#include <vector>
#include "GraphMethod.h"
#include "SegmentTree.h"
#include "Union.h"
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <list>
#include <utility>
#include <algorithm>

using namespace std;

bool BFS(Graph* graph, char option, int vertex, ofstream* fout)
{
	int n = graph->getSize();
	bool* visited = new bool[n + 1];
	fill_n(visited, n + 1, false);

	queue<int> s;
	s.push(vertex);
	visited[vertex] = true;
	int v;

	*fout << "======== BFS ========" << endl;
	if (option == 'Y')
		*fout << "Directed Graph BFS result" << endl;
	else
		*fout << "Undirected Graph BFS result" << endl;

	*fout << "startvertex: " << vertex << endl;
	*fout << vertex;
	while (!s.empty()) {
		v = s.front();
		s.pop();

		map<int, int> w;
		graph->getAdjacentEdges(v, &w, option);
		for (auto it = w.begin(); it != w.end(); it++) {
			if (visited[it->first] == false) {
				s.push(it->first);
				*fout << "->" << it->first;
				visited[it->first] = true;
			}
		}
	}
	*fout << endl << "=====================" << endl << endl;

	delete[] visited;
	return true;
}

bool DFS(Graph* graph, char option, int vertex, ofstream* fout)
{
	int n = graph->getSize();
	bool* visited = new bool[n + 1];
	fill_n(visited, n + 1, false);

	stack<int> s;
	s.push(vertex);
	visited[vertex] = true;
	int v;

	*fout << "======== DFS ========" << endl;
	if (option == 'Y')
		*fout << "Directed Graph DFS result" << endl;
	else
		*fout << "Undirected Graph DFS result" << endl;

	*fout << "startvertex: " << vertex << endl;
	*fout << vertex;
	while (!s.empty()) {
		v = s.top();

		map<int, int> w;
		graph->getAdjacentEdges(v, &w, option);
		auto it = w.begin();
		for (; it != w.end(); it++) {
			if (visited[it->first] == false) {
				s.push(it->first);
				*fout << "->" << it->first;
				visited[it->first] = true;
				break;
			}
		}
		if (it == w.end())
			s.pop();
	}
	*fout << endl << "=====================" << endl << endl;
	return true;
}

bool Kruskal(Graph* graph, ofstream* fout)
{
	vector<pair<int, pair<int, int> > >  E; // weight, start, end
	map<int, int>* T = new map<int, int>[graph->getSize() + 1];
	map<int, int> m;
	Union uni(graph->getSize());

	// complete E
	for (int i = 1; i <= graph->getSize(); i++) {
		graph->getAdjacentEdgesDirect(i, &m);
		for (auto v : m)
			E.push_back(make_pair(v.second, make_pair(i, v.first)));
	}
	quick_sort(E, 0, E.size() - 1);

	int count = 0; // count the number of T
	int cost = 0;
	while (count < graph->getSize() - 1 && !E.empty()) {
		pair<int, pair<int, int> > edge = E.back();
		E.pop_back();
		if (!uni.isUnion(edge.second.first, edge.second.second)) {
			T[edge.second.first].insert({ edge.second.second, edge.first });
			T[edge.second.second].insert({ edge.second.first, edge.first });
			count++;
			cost += edge.first;
			uni.Merge(edge.second.first, edge.second.second);
		}
	}
	if (count < graph->getSize() - 1) {
		delete[] T;
		return false;
	}

	*fout << "====== Kruskal =======" << endl;
	for (int i = 1; i <= graph->getSize(); i++) {
		*fout << '[' << i << ']' << '	';
		for (auto v : T[i])
			*fout << v.first << '(' << v.second << ')';
		*fout << endl;
	}
	*fout << "cost: " << cost << endl;
	*fout << "=====================" << endl;

	delete[] T;
	return true;
}

bool Dijkstra(Graph* graph, char option, int vertex, ofstream* fout)
{
	int n = graph->getSize();
	map<int, int> m;
	int* dist = new int[n + 1]; // length of the shortest path from v
	fill_n(dist, n + 1, 99999999);
	int* prev = new int[n + 1]; // prev vertex
	fill_n(prev, n + 1, 0);
	bool* s = new bool[n + 1]; // is included?
	fill_n(s, n + 1, false);

	// initialize
	dist[vertex] = 0;
	s[vertex] = true;
	graph->getAdjacentEdges(vertex, &m, option);
	for (auto v : m) {
		dist[v.first] = v.second;
		prev[v.first] = vertex;
	}

	for (int k = 0; k < n - 2; k++) {
		// determine n-1 paths from vertex v
			// dist[u] = minimum dist[w]
		int u = 0;
		int d = 99999999;
		for (int i = 1; i <= n; i++) {
			if (s[i] == false && dist[i] < d) {
				u = i;
				d = dist[i];
			}
		}

		s[u] = true;

		graph->getAdjacentEdges(u, &m, option);
		for (auto w : m) {
			if (s[w.first] == false) {
				if (dist[w.first] > dist[u] + w.second) {
					dist[w.first] = dist[u] + w.second;
					prev[w.first] = u;
				}
			}
		}
	}

	*fout << "====== Dijkstra =======" << endl;
	if (option == 'Y')
		*fout << "Directed Graph Dijkstra result" << endl;
	else
		*fout << "Undirected Graph Dijkstra result" << endl;
	*fout << "startvertex: " << vertex << endl;

	for (int i = 1; i <= n; i++) {
		if (i == vertex)
			continue;

		stack<int> path;
		int r;
		for (r = i; r != vertex && r != 0; r = prev[r])
			path.push(r);

		if (r == 0) {
			*fout << '[' << i << "] x" << endl;
			continue;
		}
		
		*fout << '[' << i << "] " << vertex;
		while (!path.empty()) {
			*fout << " -> " << path.top();
			path.pop();
		}
		*fout << "(" << dist[i] << ')' << endl;
	}

	*fout << "=====================" << endl << endl;

	delete[] dist;
	delete[] prev;
	delete[] s;
	return true;
}

bool Bellmanford(Graph* graph, char option, int s_vertex, int e_vertex, ofstream* fout)
{
	int n = graph->getSize();
	map<int, int> m;
	
	int* dist = new int[n + 1]; // length of the shortest path from v
	fill_n(dist, n + 1, 99999999);
	int* prev = new int[n + 1]; // prev vertex
	fill_n(prev, n + 1, 0);

	// initialize
	dist[s_vertex] = 0;
	graph->getAdjacentEdges(s_vertex, &m, option);
	for (auto v : m) {
		dist[v.first] = v.second;
		prev[v.first] = s_vertex;
	}

	for (int k = 2; k <= n - 1; k++) {
		for (int v = 1; v <= n; v++) {
			graph->getIncomingEdges(v, &m, option);
			if (m.size() == 0)
				continue;
			for (auto w : m) {
				if (dist[v] > dist[w.first] + w.second) {
					dist[v] = dist[w.first] + w.second;
					prev[v] = w.first;
				}
			}
		}
	}

	// check negative cycle by execute one more cycle
	for (int v = 1; v <= n; v++) {
		graph->getIncomingEdges(v, &m, option);
		if (m.size() == 0)
			continue;
		for (auto w : m) {
			if (dist[v] > dist[w.first] + w.second) {
				delete[] dist;
				delete[] prev;
				return false; // if dist updates, it means graph has negative cycle
			}
		}
	}

	*fout << "====== Bellman-Ford =======" << endl;
	if (option == 'Y')
		*fout << "Directed Graph Bellman-Ford result" << endl;
	else
		*fout << "Undirected Graph Bellman-Ford result" << endl;

	stack<int> path;
	int r;
	for (r = e_vertex; r != s_vertex && r != 0; r = prev[r])
		path.push(r);

	if (r == 0)
		*fout << 'x' << endl;
	else {
		*fout << s_vertex;
		while (!path.empty()) {
			*fout << " -> " << path.top();
			path.pop();
		}
		*fout << endl << "cost: " << dist[e_vertex] << endl;
	}
	*fout << "=====================" << endl << endl;

	delete[] dist;
	delete[] prev;
	return true;
}

bool FLOYD(Graph* graph, char option, ofstream* fout)
{
	const int size = graph->getSize();
	// create 2d array
	int** A = new int* [size + 1];
	for (int i = 0; i <= size; i++) {
		A[i] = new int[size + 1];
	}

	map<int, int> m;
	
	// when k == 0, initializing
	for (int i = 1; i <= size; i++) {
		graph->getAdjacentEdges(i, &m, option);
		for (int j = 1; j <= size; j++) {
			auto v = m.find(j);
			if (v != m.end())
				A[i][j] = v->second;
			else if (i == j)
				A[i][j] = 0;
			else
				A[i][j] = 99999999;
		}
	}
	for (int k = 1; k <= size; k++) {
		for (int i = 1; i <= size; i++) {
			for (int j = 1; j <= size; j++)
				A[i][j] = min(A[i][j], A[i][k] + A[k][j]);
		}
	}

	for (int i = 1; i <= size; i++) {
		// check negative cycle
		if (A[i][i] != 0) {
			// delete 2d array
			for (int i = 0; i <= size; i++) {
				delete[] A[i];
			}
			delete[] A;
			return false;
		}
	}

	// print
	*fout << "======== FLOYD ========" << endl;
	if (option == 'Y')
		*fout << "Directed Graph FLOYD result" << endl;
	else
		*fout << "Undirected Graph FLOYD result" << endl;

	// FLOYD table print
	*fout << "    ";
	for (int j = 1; j <= size; j++)
		*fout << '[' << j << "] ";
	*fout << endl;
	for (int i = 1; i <= size; i++) {
		*fout << '[' << i << ']';
		for (int j = 1; j <= size; j++) {
			fout->width(3);
			if (A[i][j] == 99999999)
				*fout << 'x' << ' ';
			else
				*fout << A[i][j] << ' ';
		}
		*fout << endl;
	}
	*fout << "==========================" << endl << endl;

	// delete 2d array
	for (int i = 0; i <= size; i++) {
		delete[] A[i];
	}
	delete[] A;
	return true;
}

bool KWANGWOON(Graph* graph, int vertex, ofstream* fout)
{
	return false;

	int size = graph->getSize();
	vector<int>* kw_graph = graph->getKwGraph();
	vector<int>* segment = new vector<int>[size + 1];

	for (int i = 1; i <= size; i++) {
		sort(kw_graph[i].begin(), kw_graph[i].end());
		init(1, 0, kw_graph[i].size() - 1, segment[i]);
	}

	*fout << "======== KWANGWOON========" << endl;
	*fout << "startvertex: 1" << endl;
	*fout << vertex;

	// delete vertex from segment[v]
	vector<int> adj = kw_graph[vertex];
	for (int i = 0; i < adj.size(); i++) { // v are accessible vertexes
	}

	for (int i = 0; i < kw_graph[vertex].size(); i++) {
		if (sum(1, 0, kw_graph[vertex].size() - 1, i, i, segment[vertex]) == 1)
			vertex = kw_graph[vertex][i];
	}

}

void quick_sort(vector<pair<int, pair<int, int> > >& e, int low, int high) {
	if (low < high) {
		if (high - low + 1 <= 6)
			insertion_sort(e, low, high);
		else {
			int i = low;
			int j = high + 1;
			int pivot = e.at(low).first;
			do {
				do i++; while (i < high && e.at(i).first > pivot);
				do j--; while (j > low + 1 && e.at(j).first < pivot);
				if (i < j) swap(e.at(i), e.at(j));
			} while (i < j);
			swap(e.at(low), e.at(j));
			quick_sort(e, low, j - 1);
			quick_sort(e, j + 1, high);
		}
	}
}

void insertion_sort(vector<pair<int, pair<int, int> > >& e, int low, int high) {
	for (int j = low; j <= high; j++) {
		pair<int, pair<int, int> > key = e[j];

		int i = j - 1;
		while (i >= low && e[i].first < key.first) {
			e[i + 1] = e[i];
			i--;
		}
		e[i + 1] = key;
	}
}