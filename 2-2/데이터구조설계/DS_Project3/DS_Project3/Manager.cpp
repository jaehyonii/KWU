#include "Manager.h"
#include "GraphMethod.h"
#include "StrSplit.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

Manager::Manager()
{
	graph = nullptr;	
	fout.open("log.txt", ios::app);
	load = 0;	// Anything is not loaded
}

Manager::~Manager()
{
	if(load)	//if graph is loaded, delete graph
		delete graph;	
	if(fout.is_open())	//if fout is opened, close file
		fout.close();	//close log.txt File
}

void Manager::run(const char* command_txt){
	ifstream fin; // Command File Input File Stream
	string cmd;
	fin.open(command_txt, ios_base::in); // File open with read mode
	
	if(!fin) { //If command File cannot be read, Print error
		fout<<"command file open error"<<endl;
		return;	//Return
	}
	
	while (getline(fin, cmd)) {
		vector<string> line = split(cmd, ' ');
		if(line[0] == "LOAD") {
			if(line.size() != 2 || !LOAD(line[1].c_str()))
				printErrorCode(100);
			load = 1;
		}
		else if (line[0] == "PRINT") {
			if (line.size() != 1 || !PRINT())
				printErrorCode(200);
		}
		else if (line[0] == "BFS") {
			if (line.size() != 3 || !mBFS(line[1][0], stoi(line[2])))
				printErrorCode(300);
		}
		else if (line[0] == "DFS") {
			if (line.size() != 3 || !mDFS(line[1][0], stoi(line[2])))
				printErrorCode(400);
		}
		else if (line[0] == "KRUSKAL") {
			if (line.size() != 1 || !mKRUSKAL())
				printErrorCode(600);
		}
		else if (line[0] == "DIJKSTRA") {
			if (line.size() != 3 || !mDIJKSTRA(line[1][0], stoi(line[2])))
				printErrorCode(700);
		}
		else if (line[0] == "BELLMANFORD") {
			if (line.size() != 4 || !mBELLMANFORD(line[1][0], stoi(line[2]), stoi(line[3])))
				printErrorCode(800);
		}
		else if (line[0] == "FLOYD") {
			if (line.size() != 2 || !mFLOYD(line[1][0]))
				printErrorCode(900);
		}
		else if (line[0] == "KWANGWOON") {
			if(line.size() != 1 || !mKwoonWoon(1))
				printErrorCode(500);
			// question
		}
		else if (line.size() != 1 || line[0] == "EXIT") {
			break;
		}
		else {
			printErrorCode(1000);
		}
	}

	fin.close();
	return;
}

bool Manager::LOAD(const char* filename)
{
	ifstream fin;	// Graph File Input File Stream
	string data;
	fin.open(filename, ios_base::in); // File open with read mode

	if (!fin || fin.peek() == EOF) // If Graph File cannot be read or is empty file
		return false; // Return

	// start making graph
	// get graph type List of Matrix
	getline(fin, data);
	bool m_Type;
	if (data == "L") m_Type = false;
	else if (data == "M") m_Type = true;
	else return false;
	
	// get graph size
	getline(fin, data);
	int m_Size = stoi(data);
	
	// create graph
	if (load != 0) {
		delete graph; // if graph already exists, delete it.
		graph = nullptr;
	}
	
	// make ListGraph
	if (m_Type == false) {
		graph = new ListGraph(m_Type, m_Size);
		int start = 0, end = 0, weight = 0;
		while (getline(fin, data)) {
			vector<string> line = split(data, ' ');
			if (line.size() == 1)
				start = stoi(line[0]);
			else if (line.size() == 2) {
				end = stoi(line[0]);
				weight = stoi(line[1]);
				graph->insertEdge(start, end, weight);
			}
			else {
				delete graph;
				graph = nullptr;
				return false;
			}
			
		}
	}
	// make MatrixGraph
	else {
		graph = new MatrixGraph(m_Type, m_Size);
		int start = 1, end;
		while (getline(fin, data) && start <= m_Size) {
			vector<string> weight = split(data, ' ');
			if (weight.size() != m_Size) {
				delete graph;
				graph = nullptr;
				return false;
			}
			for (end = 1; end <= m_Size; end++) {
				graph->insertEdge(start, end, stoi(weight[end - 1]));
			}
			start++;
		}
	}

	fout << "========LOAD=======" << endl;
	fout << "Success" << endl;
	fout << "====================" << endl << endl;
	
	fin.close();
	return true;
}

bool Manager::PRINT()
{
	if (load == 0) // if graph doesn't exist
		return false;
	
	return graph->printGraph(&fout);
}

bool Manager::mBFS(char option, int vertex)	
{
	if (vertex < 1 || vertex > graph->getSize() || (option != 'Y' && option != 'N'))
		return false;

	return BFS(graph, option, vertex, &fout);
}

bool Manager::mDFS(char option, int vertex)	
{
	if (vertex < 1 || vertex > graph->getSize() || (option != 'Y' && option != 'N'))
		return false;

	return DFS(graph, option, vertex, &fout);
}

bool Manager::mDIJKSTRA(char option, int vertex)	
{
	if (vertex < 1 || vertex > graph->getSize() || (option != 'Y' && option != 'N'))
		return false;
	
	return Dijkstra(graph, option, vertex, &fout);
}

bool Manager::mKRUSKAL()
{
	return Kruskal(graph, &fout);
}

bool Manager::mBELLMANFORD(char option, int s_vertex, int e_vertex) 
{
	if (s_vertex < 1 || s_vertex > graph->getSize())
		return false;
	if (e_vertex < 1 || e_vertex > graph->getSize())
		return false;
	if (option != 'Y' && option != 'N')
		return false;
	
	return Bellmanford(graph, option, s_vertex, e_vertex, &fout);
}

bool Manager::mFLOYD(char option)
{
	if (option != 'Y' && option != 'N')
		return false;

	return FLOYD(graph, option, &fout);
}

bool Manager::mKwoonWoon(int vertex)
{
	return KWANGWOON(graph, vertex, &fout);
}

void Manager::printErrorCode(int n)
{
	fout<<"========ERROR======="<<endl;
	fout<<n<<endl;
	fout<<"===================="<<endl << endl;
}