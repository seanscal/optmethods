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
	bool binSearch(int first, int last, string target) const;
	int partition(int left, int right);
	void quicksort(int left, int right);
	int size() const;
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

template <typename T>
void swap(T &unit1, T &unit2)
{
	T temp = unit1;
	unit1 = unit2;
	unit2 = temp;
}

int wordlist::size() const
{
	int size = words.size();
	return size;
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

bool wordlist::binSearch(int first, int last, string target) const
{
	int mid;
	string midValue;

	while(first < last)
	{
		mid = (first + last)/2;
		midValue = words[mid];
		if(target == midValue)
			return true;
		else if(target < midValue)
			last = mid;
		else 
			first = mid;
	}

	return false;
}

int wordlist::partition(int left, int right)
{
	string p = words[left];
	int i = left;
	int j = right;

	do
	{
		do{
			i++;
		}while (words[i] < p && i < right);
		do{ 
		j--;
		}while(words[j] > p);
		swap(words[i],words[j]);
	}while(i < j);
	swap(words[i],words[j]);

	swap(words[left],words[j]);
	return j;
}

void wordlist::quicksort(int left, int right)
{
	if(left < right)
	{
		int s = partition(left, right);
		quicksort(left, s-1);
		quicksort(s+1, right);
	}
}

class grid	
{
public:
	grid(string filename);
	void Display();
	int size() const { return length; }
	void CheckWords(const wordlist &list, int X, int Y, int dX, int dY) const;

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
void grid::CheckWords(const wordlist &list, //wordlist
	int X, //Starting X coordinate
	int Y, //Starting Y coordinate
	int dX, //direction moved for X direction
	int dY) const//direction moved for Y direction
{
	if (dX == 0 && dY == 0) return; //if there is no direction to move

	string CurrentWord = WordMatrix[X][Y]; //where to start in matrix

	while (CurrentWord.length() < length)
	{
		if (CurrentWord.length() > 4)
		{
			bool TF = list.binSearch(0,list.size(),CurrentWord);
			if (TF)
				cout << CurrentWord << endl; //output word if it is found
		}

		X = (X + dX + length) % length;
		Y = (Y + dY + height) % height; //Removes extra numbers to wrap around the grid
		CurrentWord += WordMatrix[X][Y];
	}
	return;
}

void findMatches(const wordlist &list, const grid &Grid)
{
	cout << "Searching for words...." << endl;

	for (int X = 0; X < Grid.size(); X++)
	{
		for (int Y = 0; Y < Grid.size(); Y++)
		{
			Grid.CheckWords(list, X, Y, 0, 1);

			Grid.CheckWords(list, X, Y, 1, 0);

			Grid.CheckWords(list, X, Y, 0, -1);
			
			Grid.CheckWords(list, X, Y, 1, -1);
			
			Grid.CheckWords(list, X, Y, 1, 1);

			Grid.CheckWords(list, X, Y, -1, -1);
			
			Grid.CheckWords(list, X, Y, -1, 0);
			
			Grid.CheckWords(list, X, Y, -1, 1);
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
	List.quicksort(0,List.size()-1);

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
	
	getch(); //wait after completed until a key press
}

