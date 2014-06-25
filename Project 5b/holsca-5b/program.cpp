// holsca-5a.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include <iostream>
#include <limits.h>
#include "d_except.h"
#include <list>
#include <fstream>
#include "d_matrix.h"
#include "graph.h"
#include "conio.h"
#include <map>
#include <queue>

using namespace std;

class maze
{
public:
	maze(ifstream &fin);

	int numRows() { return rows; };
	int numCols() { return cols; };

	void print(int, int, int, int);
	bool isLegal(int i, int j);

	void setMap(int i, int j, int n);
	int getMap(int i, int j) const;
	int getReverseMapI(int n) const;
	int getReverseMapJ(int n) const;

	void mapMazeToGraph(graph &g);

	// Added functions

	void findpathNonRecursive(graph &g, int curr, int end);//5a
	
	void findShortestPath1(graph &g, int start, int end); //5b
	void findShortestPath2(graph &g, int start, int end); //5b
	
	void printPath();
	vector<int> getNeighbors(int id, graph &g);

	void resetisPath() { isPath = false; }
	void clearcorrectPath() { correctPath.clear(); }

private:
	int rows;		// number of rows in the maze
	int cols;		// number of columns in the maze

	matrix<bool> value;

	// Added variables
	matrix<int> map;
	vector<pair<int, int>> nodepairvect;
	vector<int> correctPath;
	bool isPath;
	queue<int> PositionQueue;
	vector<int> shorter;
};

void maze::setMap(int i, int j, int n)
// Set mapping from maze cell (i,j) to graph node n. 
{
	map[i][j] = n;
	nodepairvect[n].first = i;
	nodepairvect[n].second = j;
}

int maze::getMap(int i, int j) const
// Return mapping of maze cell (i,j) in the graph.
{
	return map[i][j];
}

int maze::getReverseMapI(int n) const
// Return reverse mapping of node n to it's maze i value.
{
	return nodepairvect[n].first;
}

int maze::getReverseMapJ(int n) const
// Return reverse mapping of node n to it's maze j value.
{
	return nodepairvect[n].second;
}

maze::maze(ifstream &fin)
// Initializes a maze by reading values from fin.  Assumes that the
// number of rows and columns indicated in the file are correct.
{
	fin >> rows;
	fin >> cols;

	char x;

	value.resize(rows, cols);
	map.resize(rows, cols);
	nodepairvect.resize(rows * cols);

	for (int i = 0; i <= rows - 1; i++)
	{
		for (int j = 0; j <= cols - 1; j++)
		{
			fin >> x;
			if (x == 'O')
				value[i][j] = true;
			else
				value[i][j] = false;
		}
	}
}

void maze::print(int goalI, int goalJ, int currI, int currJ)
// Print out a maze, with the goal and current cells marked on the
// board.
{
	cout << endl;

	if (goalI < 0 || goalI > rows || goalJ < 0 || goalJ > cols)
		throw rangeError("Bad value in maze::print");

	if (currI < 0 || currI > rows || currJ < 0 || currJ > cols)
		throw rangeError("Bad value in maze::print");

	for (int i = 0; i <= rows - 1; i++)
	{
		for (int j = 0; j <= cols - 1; j++)
		{
			if (i == goalI && j == goalJ)
				cout << "*";
			else
			if (i == currI && j == currJ)
				cout << "+";
			else
			if (value[i][j])
				cout << " ";
			else
				cout << "X";
		}
		cout << endl;
	}
	cout << endl;
}

bool maze::isLegal(int i, int j)
// Return the value stored at the (i,j) entry in the maze, indicating
// whether it is legal to occupy cell (i,j).
{
	if (i < 0 || i > rows || j < 0 || j > cols)
		throw rangeError("Bad value in maze::isLegal");

	return value[i][j];
}


// This function maps the entire maze into a graph
void maze::mapMazeToGraph(graph &g)
{
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			if (isLegal(i, j))
			{
				int n = g.addNode();
				setMap(i, j, n);

				if (i != 0 && isLegal(i - 1, j))
				{

					int m = getMap(i - 1, j);
					g.addEdge(m, n);
					g.addEdge(n, m);
				}

				if (j != 0 && isLegal(i, j - 1))
				{
					int m = getMap(i, j - 1);
					g.addEdge(m, n);
					g.addEdge(n, m);
				}
			}
		}
	}
}

//This function returns a vector of all of the neighbors of the current node
vector<int> maze::getNeighbors(int id, graph &g)
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
//This function finds the correct path to the end of the maze using stacks, with no recursion needed
void maze::findpathNonRecursive(graph &g, int start, int end)
{
	g.clearVisit();
	g.clearMark();

	stack<int> shorterstack;
	stack<int> visitedstack;
	stack<edge> edges;
	stack<int> pathstack;
	vector<stack<int>> pathstackvector;
	stack<int> reversepath;

	visitedstack.push(start);
	while (!visitedstack.empty())
	{
		int top = visitedstack.top();
		vector<int> neighbors = getNeighbors(top, g);

		while (!edges.empty() && pathstack.top() != edges.top().getSource())
		{
			pathstack.pop();
		}
		pathstack.push(top);

		if (!edges.empty())
		{
			edges.pop();
		}
		visitedstack.pop();
		g.visit(top);

		if (top == end)
		{	
			g.unVisit(top);
			if (shorterstack.size() == 0)
			{
				shorterstack = pathstack;
				pathstackvector.push_back(pathstack);
			}
			else if (shorterstack.size() >= pathstack.size())
			{
				shorterstack = pathstack;
				pathstackvector.push_back(shorterstack);
			}
				
		}

		for (int i = 0; i < neighbors.size(); i++)
		{
			if (!g.isVisited(neighbors[i]))
			{
				visitedstack.push(neighbors[i]);
				edges.push(g.getEdge(top, neighbors[i]));
			}
		}
	}



	for (int i = 0; i < pathstackvector.size(); i++)
	{
		if (pathstackvector[i].size() > reversepath.size())
		{
			reversepath = pathstackvector[i];
		}
	}
	while (!correctPath.empty())
	{
		correctPath.pop_back();
	}

	while (!reversepath.empty())
	{
		int top = reversepath.top();
		reversepath.pop();
		if (g.isVisited(top))
		{
			correctPath.push_back(top);
		}
	}
}



/*************************5B FUNCTIONS**************************************/

//This function finds the shortest possible path to the goal using DFS
void maze::findShortestPath1(graph &g, int position, int goal)
{
	g.mark(position);
	g.visit(position);
	correctPath.push_back(position);
	int top;

	if (position == goal)									//if the path so far leads to the goal
	{
		if (shorter.size() == 0)
		{
			shorter = correctPath;
		}
		if (shorter.size() > correctPath.size())			//checks whether this is the shortest path to the goal so far
		{
			while (!shorter.empty())						//this piece of code is used to prevent the solution from printing backwards
				shorter.pop_back();

			while (!correctPath.empty())			
			{
				top = correctPath.back();
				correctPath.pop_back();
				shorter.push_back(top);
			}

 			correctPath = shorter;							//sets correctpath back equal to what it was before formatting
		}
	}

	else											
	{
		vector<int> neighbors = getNeighbors(position, g);	//get the neighbors

		while (!neighbors.empty())							//while there are neighbors, check if they are visited, if not then recurse.
		{
			int n = neighbors.back();
			neighbors.pop_back();

			if (!g.isVisited(n))
			{
				findShortestPath1(g, n, goal);
			}
		}
	}
	g.unVisit(position);									//unVisit the current position so that all paths are checked
	correctPath.pop_back();									
}


//This function finds the shortest possible path to the goal using BFS
void maze::findShortestPath2(graph &g, int position, int goal)
{
	g.clearMark();
	g.clearVisit();
	shorter.clear();
	queue<int> PositionQueue;
	int start = position;
	stack<int> NodePath;
	vector<int> tempPath;
	std::map<int, int> Paths;
	PositionQueue.push(position);
	bool PathTF = false;

	while (!PositionQueue.empty())									//when the positionqueue is populated
	{
		position = PositionQueue.front();							
		PositionQueue.pop();
		if (position == goal)										//check if it has reached the goal, if it has push to a path vector
		{
			NodePath.push(goal);									
			PathTF = true;
			break;
		}

		vector<int> neighbors = getNeighbors(position, g);			
		
		while (!neighbors.empty())									//while there are neighbors, check if they are visited and if not visit them
		{
			int n = neighbors.back();
			neighbors.pop_back();
			if (!g.isVisited(n))
			{
				g.visit(n);
				PositionQueue.push(n);
				Paths.insert(pair<int, int>(n, position));
			}
		}
	}

	int CurrPosition = goal;
	while (PathTF && CurrPosition != start)							//pushes the correct path onto the NodePath vector
	{
		CurrPosition = Paths.find(CurrPosition)->second;
		NodePath.push(CurrPosition);
	}

	while (!(NodePath.empty()))										//move to stack to reverse
	{
		if (g.isVisited(NodePath.top()))
			tempPath.push_back(NodePath.top());
		NodePath.pop();
	}

	while (!tempPath.empty())										//pushes back the correct path to the vector shorter for printing
	{
		shorter.push_back(tempPath.back());
		tempPath.pop_back();
	}
}



/********************************************************************************/



void maze::printPath()
{
	if (shorter.empty())
	{
		cout << "No path exists." << endl;
	}
	else
	{
		int top;
		int prev = 0;
		int previ, prevj, topi, topj;
		int moves = 0;

		while (!shorter.empty())
		{
			moves++;
			top = shorter.back();

			previ = getReverseMapI(prev);
			prevj = getReverseMapJ(prev);
			topi = getReverseMapI(top);
			topj = getReverseMapJ(top);

			print(3, numCols() - 1, topi, topj);

			shorter.pop_back();
			prev = top;
			//_getch();
		}
		cout << "Finished maze in " << moves-1 << " moves." << endl;
	}
}

// maps the maze to a graph and then solves it using the 2 different methods
int main()
{
	ifstream fin;
	string fileName = "maze1.txt";

	fin.open(fileName.c_str());
	if (!fin)
	{
		cerr << "Cannot open " << fileName << endl;
		exit(1);
	}

	try
	{
		graph mazegraph;
		while (fin && fin.peek() != 'Z')
		{
			maze m(fin);
			m.mapMazeToGraph(mazegraph);
			m.resetisPath();
			mazegraph.clearVisit();
			mazegraph.clearMark();

			int start = m.getMap(0, 0);
			int end = m.getMap(3, m.numCols() - 1);

			cout << "\n***********DFS***************\n" << endl;
			cout << "Press any Button to continue" << endl;
			_getch();
			m.findShortestPath1(mazegraph, start, end);
			m.printPath();


			cout << "\n**********BFS************\n" << endl;
			cout << "Press any Button to continue" << endl;
			_getch();
			m.findShortestPath2(mazegraph, start, end);
			m.printPath();
			_getch();
		}
	}
	catch (indexRangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}
	catch (rangeError &ex)
	{
		cout << ex.what() << endl; exit(1);
	}
}
