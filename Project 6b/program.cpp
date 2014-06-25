#include "stdafx.h"
#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <list>
#include <fstream>
#include "d_matrix.h"
#include "graph.h"
#include <queue>
#include <vector>
#include<conio.h>

using namespace std;

int const NONE = -1;

void visitNodes(int start, graph &g)
{
	bool found = false;

	g.visit(start);

	int v = 0;

	while (!found && v < g.numNodes())
	{
		if (g.isEdge(start, v) && !g.isVisited(v))
		{
			visitNodes(v, g);
		}
		v++;
	}
}

vector<int> getNeighbors(int id, graph &g)
{
	vector<int> lst;
	for (int i = 0; i < g.numNodes(); i++)
	{
		if (g.isEdge(id, i))
		{
			lst.push_back(i);
		}
	}
	return lst;
}

void findCycle(int curr, int start, bool &found, graph &g)
{
	g.mark(curr);

	vector<int> lst = getNeighbors(curr, g);

	for (int i = 0; i < lst.size(); i++)
	{
		if (start == lst[i])
		{
			continue;
		}
		if (g.isMarked(lst[i]))
		{
			found = true;
		}
		else if (!g.isVisited(lst[i]))
		{
			findCycle(lst[i], curr, found, g);
		}
	} // for

	g.unMark(curr);
	g.visit(curr);

} // findCycle

bool isCyclic(graph &g)
{
	g.clearVisit();
	g.clearMark();

	bool cycle = false;

	for (int i = 0; i < g.numNodes(); i++)
	{
		if (!g.isVisited(i))
			findCycle(i, i, cycle, g);
	}

	g.clearMark();
	g.clearVisit();

	return cycle;
}

bool isConnected(graph &g)
{
	g.clearVisit();
	g.clearMark();

	visitNodes(0, g);

	for (int i = 0; i < g.numNodes(); i++)
	{
		if (!g.isVisited(i))
		{
			return false;
		}
	}
	return true;
}

void findSpanningForest(graph &g, graph &sf)
{
	if (isConnected(g) && !isCyclic(g))
	{
		sf = g;
	}
	else
	{
		for (int i = 0; i < g.numNodes(); i++)
		{
			sf.addNode(g.getNode(i));
		}
		for (int i = 0; i < g.numNodes(); i++)
		{
			for (int j = 0; j < g.numNodes(); j++)
			{
				if (g.isEdge(i, j) && !sf.isEdge(i, j))
				{
					sf.addEdge(i, j, g.getEdgeWeight(i, j));
					sf.addEdge(j, i, g.getEdgeWeight(j, i));

					if (isCyclic(sf))
					{
						sf.removeEdge(j, i);
						sf.removeEdge(i, j);
					}
				}
			}
		}
	}
}


/*********************6B Functions****************************/

class EdgeWeightComp // Used to compare edges and see which is greater
{
public:
	bool operator() (edge &edge1, edge &edge2)
	{
		return edge1.getWeight() > edge2.getWeight();
	}
};

void primMSF(graph &g, graph &sf, int start)
//This finds the minimum spanning forest in the graph for the prim algorithm
{
	priority_queue<edge, vector<edge>, EdgeWeightComp> edgePQ;
	vector<int> neighbors = getNeighbors(start, g);

	for (int i = 0; i < neighbors.size(); i++)
	{
		edgePQ.push(g.getEdge(start, neighbors[i]));
		g.mark(start, neighbors[i]);
	}
	g.visit(start);

	int source, dest, weight;
	edge top;

	while (!edgePQ.empty())
	{
		top = edgePQ.top();
		edgePQ.pop();
		source = top.getSource();
		dest = top.getDest();
		weight = top.getWeight();

		if (!sf.isEdge(source, dest))
		{
			sf.addEdge(source, dest, weight);
			sf.addEdge(dest, source, weight);

			if (isCyclic(sf))// checks if it is a cycle
			{
				sf.removeEdge(source, dest);
				sf.removeEdge(dest, source);
			}
			else
			{
				g.visit(source);
				neighbors = getNeighbors(dest, g);

				for (int i = 0; i < neighbors.size(); i++)
				{
					if (!g.isMarked(dest, neighbors[i]))
					{
						edgePQ.push(g.getEdge(dest, neighbors[i]));
						g.mark(dest, neighbors[i]);
					}
				}
			}
		}
	}
}

void prim(graph &g, graph &msf)
{
	//implements the primMSF function
	for (int i = 0; i < g.numNodes(); i++)
	{
		msf.addNode(g.getNode(i));
	}
	for (int i = 0; i < g.numNodes(); i++)
	{
		primMSF(g, msf, i);
	}
}

void getAllEdges(int Node, graph &g, priority_queue<edge, vector<edge>, EdgeWeightComp> &EdgePQ, vector<int> &Neighbors)
{
	//Used in Kruskal function to get all of the edges in the graph


	Neighbors = getNeighbors(Node, g);
	for (int n = 0; n < Neighbors.size(); n++)
	{
		if (!(g.isVisited(Neighbors[n])))
		{
			g.visit(Neighbors[n]);
			EdgePQ.push(g.getEdge(Node, Neighbors[n]));
			getAllEdges(Neighbors[n], g, EdgePQ, Neighbors);
		}
	}

}

void kruskal(graph &g, graph &sf)
{
	priority_queue<edge, vector<edge>, EdgeWeightComp> EdgePQ;
	int source, dest, weight;
	edge Current;
	vector<int> Neighbors;

	for (int i = 0; i < g.numNodes(); i++)
	{
		sf.addNode(g.getNode(i));
	}
	g.clearVisit();
	for (int i = 0; i < g.numNodes(); i++)
	{
		if (!g.isVisited(i))
		{
			g.visit(i);
			getAllEdges(i, g, EdgePQ, Neighbors);
		}
	}//Adds all edges of the graph g to a queue in order of weight

	while (!EdgePQ.empty())
	{
		Current = EdgePQ.top();
		EdgePQ.pop();

		source = Current.getSource();
		dest = Current.getDest();
		weight = Current.getWeight();

		if (!sf.isEdge(source, dest))
		{
			sf.addEdge(source, dest, weight);
			sf.addEdge(dest, source, weight);
			if (isCyclic(sf))
			{
				sf.removeEdge(source, dest);
				sf.removeEdge(dest, source);
			}
		}
	}
}

/*********************6B Functions****************************/




int main()
{
	ifstream fin;
	stack <int> moves;
	string fileName;

	fileName = "graph4.txt";

	fin.open(fileName.c_str());
	if (!fin)
	{
		cerr << "Cannot open " << fileName << endl;
		exit(1);
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
		//Spanning Forest
		graph sf;
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

		//Prim
		graph msf;
		prim(g, msf);
		cout << endl;
		cout << msf;
		cout << "Cost with Prim Algorithm: " << msf.getTotalEdgeWeight() / 2 << endl;
		cout << "Difference: " << sf.getTotalEdgeWeight() / 2 - msf.getTotalEdgeWeight() / 2 << endl;
		
		connected = isConnected(msf);
		cyclic = isCyclic(msf);

		if (connected)
			cout << "Graph is connected" << endl;
		else
			cout << "Graph is not connected" << endl;

		if (cyclic)
			cout << "Graph contains a cycle" << endl;
		else
			cout << "Graph does not contain a cycle" << endl;
		cout << endl;

		//Kruskal
		graph kmsf;
		kruskal(g, kmsf);
		cout << endl;
		cout << kmsf;
		cout << "Cost with Kruskal Algorithm: " << kmsf.getTotalEdgeWeight() / 2 << endl;
		cout << "Difference: " << sf.getTotalEdgeWeight() / 2 - kmsf.getTotalEdgeWeight() / 2 << endl;

		connected = isConnected(kmsf);
		cyclic = isCyclic(kmsf);

		if (connected)
			cout << "Graph is connected" << endl;
		else
			cout << "Graph is not connected" << endl;

		if (cyclic)
			cout << "Graph contains a cycle" << endl;
		else
			cout << "Graph does not contain a cycle" << endl;
		cout << endl;
		_getch();
	}
	catch (indexRangeError &ex)
	{
		cout << ex.what() << endl;
	}
	catch (rangeError &ex)
	{
		cout << ex.what() << endl;
	}
}