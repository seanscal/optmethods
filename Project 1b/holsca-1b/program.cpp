// Opt_Methods_Project1b.cpp
//Sean Holmes
//Kevin Scalabrini
//used Visual Studio 2013. If there are any _getch() instances remaining, they should be changed to getch()

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
	void exceptions(int n, int m);
	const void PrintNumber();
	const int checkCorrect(const vector<int> Guess);
	const int checkIncorrect(const vector<int> Guess);
	const int getValue(int Position);
	void AddPosition(int Position);
	void setLength();
	void setRange();
	int getLength();
	int getRange();

private:
	int length;  //length of sequence
	int range;	 //range of digits
	vector<int> Sequence;  //vector of Mastermind sequence
};

const int code::getValue(int Position){
	return Sequence[Position];
}

code::code(int n, int m) //constructor
:length(n), range(m)
{}

void code::exceptions(int n, int m) //used to catch all exceptions for input into a code class
{
	try
	{
		if (n <= 0)
		{
			throw range_error("Length cannot be less than 1");
		}

		if (m <= 0)
		{
			throw range_error("Range cannot be less than 1");
		}

		if (m >= 11)
		{
			throw range_error("Range cannot be more than 11");
		}
	}
	catch (range_error)
	{
		cout << "Number out of range. Press any key to exit and please retry." << endl;
		_getch();
		exit(0);
	}
}

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

const int code::checkCorrect(const vector<int>Guess) // This function takes a guess vector and returns how many of the guesses were in the correct position
{
	int CorrectPosition = 0;
	for (int Position = 0; Position < Guess.size(); Position++)
	{
		if (Sequence[Position] == Guess[Position])
		{
			CorrectPosition++; //Goes through every position and adds to the Correct count when the values in the same position equal
		}
	}
	return CorrectPosition;
}

const int code::checkIncorrect(const vector<int>Guess)// This function takes a guess vector and returns how many of the guesses were in the sequence but in the wrong position
{
	int IncorrectPosition = 0;
	vector<bool> UsedSequencePosition((getLength() + 1), false);
	vector<bool> UsedGuessPosition(Guess.size(), false); //keeps track of what positions in the guess and code vectors have already been used. True means the value has been accounted for

	for (int Position = 0; Position < Guess.size(); Position++) //checks for values that are corect at the same position and sets the position as used, since they would have been caught in checkCorrect
	{
		if (Sequence[Position] == Guess[Position])
		{
			UsedSequencePosition[Position] = true;
			UsedGuessPosition[Position] = true;
		}
	}

	for (int Position = 0; Position < Guess.size(); Position++) //Goes through every position of the Guess vector
	{
		if (UsedGuessPosition[Position] == true) continue; //Checks if that position has been used in the guess vector and if it has, skips it.
		for (int SeqPosition = 0; SeqPosition < (getLength() + 1); SeqPosition++)
		{
			if (Guess[Position] == Sequence[SeqPosition] && (UsedSequencePosition[SeqPosition] != true)) //Goes through every position of the Sequence checking if it matches the value of the guess. Also checks if that position has been accounted for.
			{
				IncorrectPosition++; //Adds to the Correct Value, wrong Position count
				//cout << "Guess is " << Guess[Position] << "  at position  " << Position << endl <<"  Code is " << Sequence[SeqPosition] << "  at position " << SeqPosition <<endl; //Debugging tool
				UsedSequencePosition[SeqPosition] = true;
				UsedGuessPosition[Position] = true; // Marks the position as used for both vectors
				break; // Stops cycling through the sequence so it can continue going through the guess
			}
		}
	}
	return IncorrectPosition;
}

void code::AddNumber(int Num) // used to help create a random vector in CreateRandom
{
	Sequence.push_back(Num);
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

std::ostream& operator<<(std::ostream& os, code Code){ //overloads the << operator to display code class values
	for (int i = 0; i <= Code.getLength(); i++){
		os << Code.getValue(i) << ' ';
	}
	return os;
}

template <typename T>
std::ostream& operator<<(std::ostream& os, vector<T> code){ //overloads the << operator to display any type of vector values
	for (int i = 0; i <= code.getsize(); i++){
		os << code[i] << ' ';
	}
	return os;
}

class mastermind
{
public:
	mastermind(int n, int m);
	mastermind();
	void start();
private:
	code newcode;             // initialize a code object
	int MaxGuess;             // number of guesses to be played
	int length;
	int range;
};

mastermind::mastermind(int n, int m)
:newcode(n, m)
{
} //constructor 

mastermind::mastermind() //default constructor
: newcode(0, 0)
{}

void mastermind::start()
{
	int IncorrectPosition;
	int CorrectPosition;
	int m;
	int n;
	MaxGuess = 4;
	vector<int> Guess;//vector for the user's guess
	newcode.setLength();
	newcode.setRange();
	n = newcode.getLength();
	m = newcode.getRange();
	newcode.exceptions(n, m);

	newcode.CreateRandom(); // Creates the Random Mastermind sequence of entered length and range

	cout << "The secret code is " << n + 1 << " digits long, from 0 to " << m - 1 << endl;
	cout << "The secret code is: " << newcode << endl;

	for (int NumGuess = 0; NumGuess < MaxGuess; NumGuess++)// while there have been less than the predetermined amount of guesses
	{
		Guess.clear(); //resets guess vector

		cout << "Please enter a guess at the sequence, hitting enter after each digit" << endl;

		int Num;
		for (int i = 0; i < n + 1; i++) //takes in the user's guess
		{
			cout << "Digit " << i + 1 << ": ";
			cin >> Num;

			try
			{
				if (Num < 0)
				{
					throw range_error("Num cannot be less than 0");
				}
				if (Num > 10)
				{
					throw range_error("Num cannot be more than 10");
				}
			}
			catch (range_error) //catches any guesses that don't meet the mastermind game rules.
			{
				cout << "Number out of range. Press any key to exit and please retry." << endl;
				_getch();
				exit(0);
			}
			Guess.push_back(Num);
		}

		try
		{
			if (Guess.size() != n+1)
			{
				throw exception("Vectors are not the same size.");
			}
		}
		catch (range_error) //catches and guesses that exceed or don't meet the correct length
		{
			cout << "Vectors are not the same size. Press any key to exit and please retry." << endl;
			_getch();
			exit(0);
		}

		CorrectPosition = newcode.checkCorrect(Guess); //calls checkCorrect to check how many guesses are correct
		IncorrectPosition = newcode.checkIncorrect(Guess);//calls checkIncorrect to check how many guesses are in the wrong spot

		if (CorrectPosition == (n + 1))// if, else if, else detects whether maximum guesses are reached and whether the guess was correct
		{
			cout << "Congrats, you are right, Correct Answer is ";
			cout << newcode;
			break;
		}
		else if (CorrectPosition != n && NumGuess == MaxGuess)
		{
			cout << "Correct Answer is ";
			cout << newcode;
		}
		else
		{
			cout << "Number of correct digits in the correct position are " << CorrectPosition << endl;
			cout << "Number of correct digits in the wrong position are " << IncorrectPosition << endl << endl;
		}
	}
}

void main()// calls all other functions which run the project to completion
{
	mastermind newGame;
	newGame.start();

	_getch(); //pause so user can see output.
}
