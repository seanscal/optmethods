// Project 6
// Assumes that directed edges in both directions (x,y) and (y,x) are present in the input file.
//

#include"stdafx.h"
#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <list>
#include <fstream>
#include "d_matrix.h"
#include "graph.h"
#include <queue>
#include <vector>
#include <conio.h>

using namespace std;

int const NONE = -1;  // Used to represent a node that does not exist

vector<int> getNeighbors(int id, graph &g)
{
	vector<int> neighbors;
	for (int i = 0; i < g.numNodes(); i++)
	{
		if (g.isEdge(id, i))
		{
			neighbors.push_back(i);
		}
	}
	return neighbors;
}

bool checkCycle(int Position, graph &g)
{
	if (!g.isVisited(Position))
	{
		g.visit(Position);
		vector<int> neighbors = getNeighbors(Position, g);
		int prev = Position;
		for (int j = 0; j < neighbors.size(); j++)
		{
			int NewPos = neighbors[j];
			if (g.isVisited(NewPos) && NewPos != prev)
				return false;
			else
				checkCycle(NewPos, g);
		}
		return true;
	}
	else
		return true;
}

bool isCyclic(graph &g)
{
	bool Cyclic = true;
	g.clearVisit();

	int NumNodes = g.numNodes();
	for (int i = 0; i < NumNodes; i++)
	{
		Cyclic = Cyclic && checkCycle(i, g);
	}
	return Cyclic;
}

void findNewEdges(int Position, graph &g, graph &sf)
{
	if (!g.isVisited(Position))
	{
		g.visit(Position);
		vector<int> neighbors = getNeighbors(Position, g);
		for (int j = 0; j < neighbors.size(); j++)
		{
			int CurrNode = neighbors[j];
			if (!g.isVisited(CurrNode))
			{
				g.visit(CurrNode);
				sf.addEdge(Position, j);
				findNewEdges(j, g, sf);
			}
		}
	}
}

void findSpanningForest(graph &g, graph &sf)
{
	g.clearVisit();
	int NumNodes = g.numNodes();
	for (int i = 0; i <NumNodes; i++)
	{
		findNewEdges(i, g, sf);
	}
}

void Visit(int start, graph &g) // used in isConnected to visit nodes
{
	g.visit(start);
	int v = 0;
	while (v < g.numNodes())
	{
		if (g.isEdge(start, v) && !g.isVisited(v))
		{
			Visit(v, g);
		}
		v++;
	}
} 

bool isConnected(graph &g)
{
	// Returns true if the graph g is connected.  Otherwise returns false.
		g.clearVisit();
		g.clearMark();

		Visit(0, g);	

		for (int i = 0; i < g.numNodes(); i++)
		{
			if (!g.isVisited(i))
			{
				return false;
			}
		} 
		return true;
}


int main()
{
	char x;
	ifstream fin;
	stack <int> moves;
	string fileName;

	fileName = "graph1.txt";

	fin.open(fileName.c_str());
	if (!fin)
	{
		cerr << "Cannot open " << fileName << endl;
		exit(1);
		_getch();
	}

	try

	{
		cout << "Reading graph" << endl;
		graph g(fin);

		cout << g;

		bool connected;
		bool cyclic;

		connected = isConnected(g);
		cyclic = isCyclic(g);

		if (connected)
			cout << "Graph is connected" << endl;
		else
			cout << "Graph is not connected" << endl;

		if (cyclic)
			cout << "Graph contains a cycle" << endl;
		else
			cout << "Graph does not contain a cycle" << endl;

		cout << endl;

		cout << "Finding spanning forest" << endl;

		// Initialize an empty graph to contain the spanning forest
		graph sf(g.numNodes());
		findSpanningForest(g, sf);

		cout << endl;

		cout << sf;

		cout << "Spanning forest weight: " << sf.getTotalEdgeWeight() / 2 << endl;

		connected = isConnected(sf);
		cyclic = isCyclic(sf);

		if (connected)
			cout << "Graph is connected" << endl;
		else
			cout << "Graph is not connected" << endl;

		if (cyclic)
			cout << "Graph contains a cycle" << endl;
		else
			cout << "Graph does not contain a cycle" << endl;

		cout << endl;
	}
	catch (indexRangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}
	catch (rangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}
	_getch();
}
