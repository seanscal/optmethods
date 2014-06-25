// Opt_Methods_Project1a.cpp
//Sean Holmes
//Kevin Scalabrini
//Any commented out code is a start to project 1b


#include "stdafx.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <ctime>
#include <cstdlib> 
#include <conio.h>

using namespace std;

class code // Initialization of the code class
{
	public:
		code(int n, int m);
		void CreateRandom();
		void AddNumber(int Num);
		const void PrintNumber();
		const int checkCorrect(const vector<int> Guess);
		const int getValue(int Position);
		void AddPosition(int Position);
		void setLength();
		void setRange();
		int getLength();
		int getRange();		
		//const bool CompareVal(int Position, vector<int> Guess);
		//const bool NotInVec(int Position, vector<int> UsedPosition);
	private:
		int length;  //length of sequence
		int range;	 //range of digits
		vector<int> Sequence;  //vector of Mastermind sequence
		//vector<int> UsedPosition; 
};

code::code(int n, int m) //constructor
	:length(n), range(m)
{}

void code::setLength()// sets the length of the sequence
{
	cout << "Please enter the sequence length" << endl;
	cin >> length;
    length -= 1;

}

void code::setRange()// sets the range of the sequence
{
	cout << "Please enter the range of the digits in the sequence (must be less than 11)" << endl;
	cin >> range;
}

int code::getLength()// gets the length of the sequence
{
	return length;
}

int code::getRange()// gets the range of the sequence
{
	return range;
}

void main()// calls all other functions which run the project to completion
{
	//int CorrectVal;
	int CorrectPosition;
	int MaxGuess = 4;
	vector<int> Guess;//vector for the user's guess
	
	code Sequence(0,0);
	Sequence.setLength(); 
	Sequence.setRange();
	Sequence.CreateRandom();
	// Creates the Random Mastermind sequence of entered length and range

	int n = Sequence.getLength();
	int m = Sequence.getRange();
	for(int NumGuess = 0; NumGuess < MaxGuess; NumGuess++)// while there have been less than the predetermined amount of guesses
	{
		//CorrectVal = 0;
		Guess.clear(); //resets guess vector
		CorrectPosition = 0;
		
		cout << "Please enter a guess at the sequence, hitting enter after each digit"<<endl;
	
		int Num;
		for(int i = 0; i <= n; i++) //takes in the user's guess
		{
			cin >> Num; 
			Guess.push_back(Num);
		}

/*		for(int Position = 0; Position < Guess.size(); Position++)
		{
			if (Sequence.CompareVal(Position, Guess))
			{
				CorrectVal++;
			}
		}
		*/
		CorrectPosition = Sequence.checkCorrect(Guess); //calls checkCorrect to check how many guesses are correct

		if(CorrectPosition == (n+1))// if, else if, else detects whether maximum guesses are reached and whether the guess was correct
		{
			cout << "Congrats, you are right, Correct Answer is ";
			Sequence.PrintNumber();
			break;
		}
		else if (CorrectPosition != n && NumGuess == MaxGuess) 
		{
			cout << "Correct Answer is ";
			Sequence.PrintNumber();
		}
		else
		{
			cout << "Number of correct digits in the correct position are "<<CorrectPosition<<endl;
			//cout << "Number of correct digits in the wrong position are "<<CorrectVal<<endl;
		}
	}
	
	getch(); //pause so user can see output.
}


const int code::checkCorrect(const vector<int>Guess) // This function takes a guess vector and returns how many of the guesses were in the correct position
{
	int CorrectPosition = 0;
	for(int Position = 0; Position < Guess.size(); Position++)
		{
			if (Sequence[Position] == Guess[Position])
			{
				CorrectPosition ++;
			}
		}
	return CorrectPosition;
}

void code::AddNumber(int Num) // used to help create a random vector in CreateRandom
{
	Sequence.push_back(Num);
}

const void code::PrintNumber()//Prints out the sequence, used for when guessed correctly or max is reached
{
	for(vector<int>::const_iterator i = Sequence.begin(); i != Sequence.end(); ++i)
    cout << *i << ' ';
}

void code::CreateRandom()// Creates the random vector that the user has to guess
{
	srand(time(NULL));
	int n = getLength();
	int m = getRange();

	for (int i = 0; i <= n; i++){
		
		int RanNum = rand() % m;

		AddNumber(RanNum);
	}
}

/*const int code::getValue(int Position)
{
	int Val = Sequence[Position];
	return Val;
}*/
/* const bool code::CompareVal(int Position, vector<int>Guess)
{
	for(int i = 0; i < Guess.size(); i++){
		if(Guess[Position] == Sequence[i] && NotInVec(i, UsedPosition)){
			AddPosition(i);
			return true;
		}
	}
	return false;
}
*/
/*
const bool code::NotInVec(int Position, vector<int> UsedPosition)
{
	for(int i = 0; i < UsedPosition.size(); i++){
		if (Position == UsedPosition[i])
			return false;
	}
	return true;
}
void code::AddPosition(int Position)
{
	UsedPosition.push_back(Position);
}
*/