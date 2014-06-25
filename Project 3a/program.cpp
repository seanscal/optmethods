// code developed in visual studio 2010 and 2013
// not yet optimized, will take a long time to execute right now.
//

#include "stdafx.h"
#include <vector>
#include <string>
#include <fstream>
#include<iostream>
#include "d_matrix.h"
#include "conio.h"

using namespace std;

class wordlist
{
public:
	wordlist(string filename); //constructor
	bool lookUp(string target) const;
private:
	vector<string> words;
};

wordlist::wordlist(string filename) //constructor
{
	ifstream file;
	file.open(filename.c_str());
	cout << "Loading Dictionary..." << endl;
	string word;
	string PrevWord = "";
	while (getline(file, word))
	{
		if (word != PrevWord){
			words.push_back(word);
			PrevWord = word;
		}
	}
}

bool wordlist::lookUp(string target) const  //checks if a word is in the dictionary given
{
	for (int i = 0; i < words.size(); i++){
		if (words[i] == target){
			return true;
			break;
		}
	}
	return false;
}

class grid	
{
public:
	grid(string filename);
	void Display();
	int size() const { return length; }
	friend void CheckWords(const wordlist &list, const grid &Grid, int X, int Y, int dX, int dY);

private:
	matrix<string> WordMatrix;
	int length;
	int height;
};

grid::grid(string inputfile)  //grid constructor
{
	ifstream file;
	string newfile = inputfile;  //input file is a grid (wordsearch)
	file.open(newfile.c_str());
	if (!file)				// error if no file
	{
		throw fileOpenError(newfile);
	}

	cout << "Loading grid..." << endl;
	string xlength;
	string ylength;
	file >> xlength >> ylength;	//there are two coordinates on top of files, this gets them
	length = atoi(xlength.c_str());
	height = atoi(ylength.c_str()); // converts the coordinates to ints

	WordMatrix = matrix<string>(length, height); //create matrix
	for (int j = 0; j < length; j++)
	{
		for (int i = 0; i < height; i++)
		{
			file >> WordMatrix[j][i]; //puts file into that matrix
		}
	}

	file.close();
}

void grid::Display()  //displays the loaded matrix
{
	for (int j = 0; j < WordMatrix.rows(); j++)
	{
		for (int i = 0; i < WordMatrix.cols(); i++)
		{
			cout << WordMatrix[j][i] << ' ';
		}

		cout << endl;
	}
}

//Checks for words in one particular direction
void CheckWords(const wordlist &list, //wordlist
	const grid &Grid, //Grid of the words
	int X, //Starting X coordinate
	int Y, //Starting Y coordinate
	int dX, //direction moved for X direction
	int dY) //direction moved for Y direction
{
	if (dX == 0 && dY == 0) return; //if there is no direction to move

	string CurrentWord = Grid.WordMatrix[X][Y]; //where to start in matrix

	while (CurrentWord.length() < Grid.length)
	{
		if (CurrentWord.length() > 4)
		{
			if (list.lookUp(CurrentWord))
				cout << CurrentWord << endl; //output word if it is found
		}

		X = (X + dX + Grid.length) % Grid.length;
		Y = (Y + dY + Grid.height) % Grid.height; //Removes extra numbers to wrap around the grid
		CurrentWord += Grid.WordMatrix[X][Y];
	}
}

void findMatches(const wordlist &list, const grid &Grid)
{
	cout << "Searching for words...." << endl;

	for (int X = 0; X < Grid.size(); X++)
	{
		for (int Y = 0; Y < Grid.size(); Y++)
		{
			CheckWords(list, Grid, X, Y, 0, -1);
			CheckWords(list, Grid, X, Y, 0, 1);

			CheckWords(list, Grid, X, Y, 1, -1);
			CheckWords(list, Grid, X, Y, 1, 0);
			CheckWords(list, Grid, X, Y, 1, 1);

			CheckWords(list, Grid, X, Y, -1, -1);
			CheckWords(list, Grid, X, Y, -1, 0);
			CheckWords(list, Grid, X, Y, -1, 1);
			//Runs through all the directions for CheckWords
		}
	}
}

void testSearch(const wordlist &List)
{
	string filename;
	cout << "Please enter the grid file name (input15, input30, or input50): ";
	cin >> filename;
	filename += ".txt";
	grid Grid(filename);
	Grid.Display();

	findMatches(List, Grid);

	cout << "---------------------------------------" << endl << "Done Searching";
}

int main()
{
	wordlist List("wordlist.txt");  //gets the list we were given as a dictionary

	try //catches errors
	{
		testSearch(List);
	}

	catch (fileOpenError &FOE)
	{
		cout << FOE.what() << endl;
	}
	catch (indexRangeError &IRE)
	{
		cout << IRE.what() << endl;
	}

	_getch(); //wait after completed until a key press
}


