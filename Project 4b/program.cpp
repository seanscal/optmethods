//HolSca-4b  Developed in Visual Studio 2010 and 2013

#include "stdafx.h"
#include <iostream>
#include <limits.h>
#include "d_matrix.h"
#include "d_except.h"
#include <list>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include "conio.h"
#include <stdio.h>
#include <stdlib.h>

using namespace std;

typedef int ValueType;		// The type of the value in a cell
const int Blank = -1;		// Indicates that a cell is blank

const int SquareSize = 3;	//  The number of cells in a small square
//  (usually 3).  The board has
//  SquareSize^2 rows and SquareSize^2
//  columns.

const int BoardSize = SquareSize * SquareSize;

const int MinValue = 1;
const int MaxValue = 9;

int numSolutions = 0;

int originalarray[9][9];
int endingArray[9][9];

int total = 0;
int boardCounter = 0;
class board
{
public:
	board(int);
	void clear();
	void initialize(ifstream &fin); //changes made in this function
	void print();
	bool isBlank(int, int);
	ValueType getCell(int, int);
	int squareNumber(int i, int j);

	// Added-4a
	void printConflicts();
	bool conflicts(int i, int j, int val);
	void setCell(int i, int j, int val);
	void ClearCell(int i, int j);
	bool solvedBoard();

	

	/***************THESE ARE 4B FUNCTIONS*******************/

	int getRecursionCount() { return recursionCounter; }//get number of recursions
	void clearRecursionCount() { recursionCounter = 0; }//reset the number for each board
	int goodPositionCheck(int i, int j, int value);
	int redoCheck(int x, int y);
	void printArray();

	/***************THESE ARE 4B FUNCTIONS*******************/

private:
	// Added-4a
	void ConflictUpdate(int i, int j, int num, bool val);


	// The following matrices go from 1 to BoardSize in each
	// dimension.  ie. they are each (BoardSize+1)x(BoardSize+1)
	matrix<ValueType> value;

	matrix<bool> rows;		// matrix of each row conflict
	matrix<bool> cols;		// matrix of each column conflict
	matrix<bool> sqrs;		// matrix of each square conflict

	matrix<int> output;		// output matrix

	int recursionCounter; //counts number of recursions in program
};

board::board(int sqSize)
: value(BoardSize + 1, BoardSize + 1),
rows(BoardSize + 1, BoardSize + 1),
cols(BoardSize + 1, BoardSize + 1),
sqrs(BoardSize + 1, BoardSize + 1)
// Board constructor
{
}

bool board::conflicts(int i, int j, int Val)
{
	int SqrNum = squareNumber(i, j);
	return rows[i][Val] || cols[j][Val] || sqrs[SqrNum][Val];
}

void board::ConflictUpdate(int i, int j, int val, bool Bool)
{
	rows[i][val] = Bool;
	cols[j][val] = Bool;
	sqrs[squareNumber(i, j)][val] = Bool;
}

void board::clear()
{
	for (int i = 1; i <= BoardSize; i++)
	{
		for (int j = 1; j <= BoardSize; j++)
		{
			ClearCell(i, j);
		}
	}
}

bool board::solvedBoard()
{
	bool Solved = true;
	for (int i = 1; i <= BoardSize; i++)
	{
		for (int j = 1; j <= BoardSize; j++)
		{
			Solved = Solved && !(isBlank(i, j));
			if (!Solved)
				return false;
		}
	}
	return true;
}

void board::initialize(ifstream &fin)
// Read a Sudoku board from the input file.
{
	char ch;
	clear();	
	for (int i = 1; i <= BoardSize; i++)
	{
		for (int j = 1; j <= BoardSize; j++)
		{
			fin >> ch;

			if (ch != '.')// If the read char is not Blank
			{
				if (conflicts(i, j, ch - '0'))
				{
					throw rangeError("conflict on initialization");
				}
				setCell(i, j, ch - '0');	// Convert char to int
				originalarray[i - 1][j - 1] = ch - '0';
			}

			else if (ch == '.')
			{
				setCell(i, j, 0);		
				originalarray[i - 1][j - 1] = 0;
			}
		}
	} 
} 

int board::squareNumber(int i, int j)
// Return the square number of cell i,j (counting from left to right,
// top to bottom.  Note that i and j each go from 1 to BoardSize
{
	// Note that (int) i/SquareSize and (int) j/SquareSize are the x-y
	// coordinates of the square that i,j is in.  

	return SquareSize * ((i - 1) / SquareSize) + (j - 1) / SquareSize + 1;
}

ostream &operator<<(ostream &ostr, vector<int> &v)
// Overloaded output operator for vector class.
{
	for (int i = 0; i < v.size(); i++)
		ostr << v[i] << " ";
	ostr << endl;
	return ostr;
}

ValueType board::getCell(int i, int j)
// Returns the value stored in a cell.  Throws an exception
// if bad values are passed.
{
	if (i >= 1 && i <= BoardSize && j >= 1 && j <= BoardSize)
		return value[i][j];
	else
		throw rangeError("bad value in getCell");
}

void board::setCell(int i, int j, int Val)
{
	if (i >= 1 && i <= BoardSize && j >= 1 && j <= BoardSize)
	{
		value[i][j] = Val;
		ConflictUpdate(i, j, Val, true);
	}
	else
		throw rangeError("bad value in getCell");
}
void board::ClearCell(int i, int j)
{
	if (i >= 1 && i <= BoardSize && j >= 1 && j <= BoardSize)
	{
		ConflictUpdate(i, j, value[i][j], false);
		value[i][j] = 0;
	}
	else
		throw rangeError("bad value in getCell");
}

bool board::isBlank(int i, int j)
// Returns true if cell i,j is blank, and false otherwise.
{
	if (i < 1 || i > BoardSize || j < 1 || j > BoardSize)
		throw rangeError("bad value in setCell");
	if (value[i][j] == 0)
		return true;
	else
		return false;
}

void board::print()
// Prints the current board.
{
	for (int i = 1; i <= BoardSize; i++)
	{
		if ((i - 1) % SquareSize == 0)
		{
			cout << " -";
			for (int j = 1; j <= BoardSize; j++)
				cout << "---";
			cout << "-";
			cout << endl;
		}
		for (int j = 1; j <= BoardSize; j++)
		{
			if ((j - 1) % SquareSize == 0)
				cout << "|";
			if (!isBlank(i, j))
				cout << " " << getCell(i, j) << " ";
			else
				cout << "   ";
		}
		cout << "|";
		cout << endl;
	}

	cout << " -";
	for (int j = 1; j <= BoardSize; j++)
		cout << "---";
	cout << "-";
	cout << endl;
}

void board::printConflicts()
{
	cout << "Conflicts: Rows        ";
	cout << "Columns                 ";
	cout << "Squares" << endl;
	int j = 1;
	for (int i = 1; i < value.rows(); i++)
	{
		for (j = 1; j < value.cols(); j++)
		{
			cout << rows[i][j] << " ";
		}
		cout << "  |  ";
		for (j = 1; j < value.cols(); j++)
		{
			cout << cols[i][j] << " ";
		}
		cout << "  |  ";
		for (j = 1; j < value.cols(); j++)
		{
			cout << sqrs[i][j] << " ";
		}
		cout << endl;
	}
}



/*************4B FUNCTIONS***************************/


//Makes sure that the value pushed into it can be put into that certain space     
int board::goodPositionCheck(int i, int j, int value)

{	
	int newi = (i / 3) * 3;
	int newj = (j / 3) * 3;
	
	//This nested for loop checks one of the 9 squares to see if the number is in it
	for (int x = newi; x < newi + 3; x++)
	{
		for (int y = newj; y < newj + 3; y++)
		{
			if (endingArray[x][y] == value)
			{
				return 0;
			} 
		} 
	} 

	//This then checks whether the number is in the row or column. It will be placed in the spot if not.
	for (int k = 0; k < 9; k++)
	{
		if (endingArray[k][j] == value || endingArray[i][k] == value)
		{
			return 0;
		}
	} 

	return value;
}

//this redoes the check above until the array is filled with numbers which can be used
int board::redoCheck(int x, int y)
{
	recursionCounter++;	
	int i;

	if (endingArray[x][y] == 0)
	{
		for (i = 1; i < 10; i++)
		{
			int valAtSpace = goodPositionCheck(x, y, i);

			if (valAtSpace != 0)
			{
				endingArray[x][y] = valAtSpace;

				if (x == 8 && y == 8) { return 1; }
				else if (x == 8)
				{
					if (redoCheck(0, y + 1)) { return 1; }
				}
				else
				{
					if (redoCheck(x + 1, y)) { return 1; }
				}
			} 
		} 

		if (i == 10)
		{
			if (endingArray[x][y] != originalarray[x][y])
			{
				endingArray[x][y] = 0;
			}
			return 0;
		} 
	}
	else
	{
		if (x == 8 && y == 8) { return 1; }
		else if (x == 8)
		{
			if (redoCheck(0, y + 1)) { return 1; }
		}
		else
		{
			if (redoCheck(x + 1, y)) { return 1; }
		}
	} 
} 

// Prints the endArray once it has been completely filled with non-'0' numbers;
void board::printArray()
{
	cout << "+-";
	for (int j = 1; j <= BoardSize; j++)
	{
		cout << "---";
	}
	cout << "-+";
	cout << endl;

	for (int i = 1; i <= BoardSize; i++)
	{
		if (i == 4 || i == 7)
		{
			cout << "|";
			for (int j = 1; j < SquareSize; j++)
			{
				cout << "---------+";
			}
			cout << "---------|";
			cout << endl;
		}

		for (int j = 1; j <= BoardSize; j++)
		{
			if ((j - 1) % SquareSize == 0)
			{
				cout << "|";
			}
			cout << " " << endingArray[i - 1][j - 1] << " ";
		}
		cout << "|";
		cout << endl;
	}

	cout << "+-";
	for (int j = 1; j <= BoardSize; j++)
	{
		cout << "---";
	}
	cout << "-+";
	cout << endl;
}



/*************4B FUNCTIONS***************************/

int main()
{
	ifstream fin;

	// Read the sample grid from the file.
	string fileName = "sudoku.txt";

	fin.open(fileName.c_str());
	if (!fin)
	{
		cerr << "Cannot open " << fileName << endl;
		exit(1);
	}

	try
	{
		time_t start, end;
		double executeTime;
		board myBoard(SquareSize);

		time(&start);
		while (fin && fin.peek() != 'Z')
		{
			boardCounter++;
			myBoard.clearRecursionCount();
			myBoard.initialize(fin);

			for (int i = 0; i < 9; i++)
			{
				for (int j = 0; j < 9; j++)
				{
					endingArray[i][j] = originalarray[i][j];
				} 
			} 

			cout << "\nBoard " << boardCounter << endl;
			myBoard.print();				

			myBoard.redoCheck(0, 0);
			cout << "\nSolved Board  " << boardCounter << endl;
			myBoard.printArray();
			//_getch();

			cout << "Number of calls: " << myBoard.getRecursionCount() - 1 << endl;
			total += myBoard.getRecursionCount()-1;
		} 
		time(&end);
		cout << "Average number of recursive calls: " << total / boardCounter << endl;

		executeTime = difftime(end, start);

		int intTime = (int)executeTime;
		int minTime = intTime / 60;	
		int secTime = intTime % 60;

		cout << "You have solved " << boardCounter << " boards.";
		cout<<"It took "<<minTime<< " minutes and " <<secTime<<" seconds.";
		_getch();
	} 
	catch (rangeError &ex)
	{
		cout << ex.what() << endl;
		exit(1);
	}
} 
