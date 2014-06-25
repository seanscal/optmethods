// Declarations and functions for project #4

#include "stdafx.h"
#include <iostream>
#include <limits.h>
#include "d_matrix.h"
#include "d_except.h"
#include <list>
#include <fstream>
#include <conio.h>

using namespace std;

typedef int ValueType; // The type of the value in a cell
const int Blank = -1;  // Indicates that a cell is blank
 
const int SquareSize = 3;  //  The number of cells in a small square
                           //  (usually 3).  The board has
                           //  SquareSize^2 rows and SquareSize^2
                           //  columns.

const int BoardSize = SquareSize * SquareSize;

const int MinValue = 1;
const int MaxValue = 9;

int numSolutions = 0;

class board
// Stores the entire Sudoku board
{
   public:
      board(int);
      void clear();
      void initialize(ifstream &fin);
      void print();
      bool isBlank(int, int);
      ValueType getCell(int, int);

	  //Added
	  void setCell(int i, int j, int Val);
	  void printConflicts();
	  bool conflicts(int i, int j, int Val);
	  void ClearCell(int i, int j);
	  bool solvedBoard();
	  int board::squareNumber(int i, int j);

   private:
	  //Added
	   void ConflictUpdate(int i, int j, int val, bool Bool);
	   //Conflict Matrices
	   matrix<bool> rows;
	   matrix<bool> cols;
	   matrix<bool> sqrs;

      // The following matrices go from 1 to BoardSize in each
      // dimension.  I.e. they are each (BoardSize+1) X (BoardSize+1)
	  matrix<int> Conflicts;
      matrix<ValueType> value;
};

board::board(int sqSize)
   : value(BoardSize+1,BoardSize+1),
   rows(BoardSize+1, BoardSize+1),
   cols(BoardSize+1, BoardSize+1),
   sqrs(BoardSize+1, BoardSize+1)
// Board constructor
{
}

bool board::conflicts(int i, int j, int Val)
{
	int SqrNum = squareNumber(i,j);
	return rows[i][Val] || cols[j][Val] || sqrs[SqrNum][Val];
}

void board::ConflictUpdate(int i, int j, int val, bool Bool)
{
	int SqrNum = squareNumber(i,j);
	rows[i][val] = Bool;
	cols[j][val] = Bool;
	sqrs[SqrNum][val] = Bool;
}

void board::clear()
{
	for (int i = 1; i <= BoardSize; i++)
	{
      for (int j = 1; j <= BoardSize; j++)
      {
		  ClearCell(i,j);
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
		  Solved = Solved && !(isBlank(i,j));
		  if(!Solved)
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
      for (int j = 1; j <= BoardSize; j++)
      {
		 fin >> ch;

			 // If the read char is not Blank
		 if (ch != '.')
		 {
			 if(conflicts(i,j,ch-'0'))
				 throw rangeError("Conflict upon Initialization");

			setCell(i,j,ch-'0');   // Convert char to int
		 }
		 else if(ch == '.')
			setCell(i,j,0);
      }
}

int board::squareNumber(int i, int j)
// Return the square number of cell i,j (counting from left to right,
// top to bottom.  Note that i and j each go from 1 to BoardSize
{
   // Note that (int) i/SquareSize and (int) j/SquareSize are the x-y
   // coordinates of the square that i,j is in.  

   return SquareSize * ((i-1)/SquareSize) + (j-1)/SquareSize + 1;
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
   if(value[i][j] == 0)
	   return true;
   else
	   return false;
}

void board::print()
// Prints the current board.
{
   for (int i = 1; i <= BoardSize; i++)
   {
      if ((i-1) % SquareSize == 0)
      {
		 cout << " -";
		 for (int j = 1; j <= BoardSize; j++)
			 cout << "---";
			 cout << "-";
		 cout << endl;
      }
      for (int j = 1; j <= BoardSize; j++)
      {
		 if ((j-1) % SquareSize == 0)
			cout << "|";
		 if (!isBlank(i,j))
			cout << " " << getCell(i,j) << " ";
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
	   for(j = 1; j < value.cols(); j++)
	   {
		   cout << rows[i][j] << " ";
	   }
	   cout << "  |  ";
	   for(j = 1; j < value.cols(); j++)
	   {
		   cout << cols[i][j] << " ";
	   }
	   cout << "  |  ";
	   for(j = 1; j < value.cols(); j++)
	   {
		   cout << sqrs[i][j] << " ";
	   }
	   cout << endl;
	}
}

void main()
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
      board b1(SquareSize);

      while (fin && fin.peek() != 'Z')
      {
		 b1.initialize(fin);
		 b1.print();
		 b1.printConflicts();
		 if(b1.solvedBoard())
			cout << "Board is solved" << endl;
		 else 
			 cout << "Board is NOT solved" << endl;
      }
   }
   catch  (indexRangeError &ex)
   {
      cout << ex.what() << endl;
      exit(1);
   }
   getch();
}

