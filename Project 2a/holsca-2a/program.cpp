
//Used Microsoft Visual Studio 2013, program is functional in this dev environment.
//May need small syntax changes to accomodate any version changes since lab version of Visual Studio

#include "stdafx.h"
#include "d_except.h"
#include "d_node.h"
#include <string>
#include <vector>
#include "iostream"
#include "conio.h"


using namespace std;

class card
{
public:
	card(int n, int m);  //constructor. n = value, m = suit
	void setvalue(int val);
	void setsuit(int newsuit);
	const int getvalue();
	const int getsuit();
	friend ostream &operator<< (ostream &ostr, const card &Card);

private:
	int value;
	int Suit;
};

card::card(int n, int m)// constructor, sets value and Suit to n and m, checks for exceptions
:value(n), Suit(m)
{
	try
	{
		if (n<2 || n > 14)
			throw rangeError("Invalid Value");
		if (m >3 || m <0)
			throw rangeError("Invalid Suit");
	}
	catch (rangeError){
		cout << "Invalid Suit or Value";
	}
}

void card::setvalue(int val) // sets value and checks for range errors
{
	value = val;
	try
	{
		if (val<2 || val > 14)
			throw rangeError("Invalid Value");
	}
	catch (rangeError){
		cout << "Invalid Value";
	}
}

void card::setsuit(int newsuit) // sets Suit and checks for range errors
{
	Suit = newsuit;

	try
	{
		if (newsuit >3 || newsuit <0)
			throw rangeError("Invalid Suit");
	}
	catch (rangeError){
		cout << "Invalid Suit";
	}
}

const int card::getvalue() // gets value
{
	return value;
}

const int card::getsuit() // gets suit
{
	return Suit;
}
ostream &operator<< (ostream &ostr, const card &Card) // overloaded print operator
{

	/* 
		Uses switch and case to go through the 4 suits and set the royal cards
		or prints out the values of the number and the suit depending on the case.
		Uses the card class to get the values and suit.
	*/
	switch (Card.Suit)
	{  
		
		
	case 0: 
		switch (Card.value){
		case 14:
			ostr << "Ace of Clubs" << endl;
			return ostr;
		case 13:
			ostr << "King of Clubs" << endl;
			return ostr;
		case 12:
			ostr << "Queen of Clubs" << endl;
			return ostr;
		case 11:
			ostr << "Jack of Clubs" << endl;
			return ostr;
		default:
			ostr << Card.value << " of Clubs" << endl;
			return ostr;
		}
	case 1:
		switch (Card.value){
		case 14:
			ostr << "Ace of Hearts" << endl;
			return ostr;
		case 13:
			ostr << "King of Hearts" << endl;
			return ostr;
		case 12:
			ostr << "Queen of Hearts" << endl;
			return ostr;
		case 11:
			ostr << "Jack of Hearts" << endl;
			return ostr;
		default:
			ostr << Card.value << " of Hearts" << endl;
			return ostr;
		}
	case 2:
		switch (Card.value){
		case 14:
			ostr << "Ace of Diamonds" << endl;
			return ostr;
		case 13:
			ostr << "King of Diamonds" << endl;
			return ostr;
		case 12:
			ostr << "Queen of Diamonds" << endl;
			return ostr;
		case 11:
			ostr << "Jack of Diamonds" << endl;
			return ostr;
		default:
			ostr << Card.value << " of Diamonds" << endl;
			return ostr;
		}
	case 3:
		switch (Card.value){
		case 14:
			ostr << "Ace of Spades" << endl;
			return ostr;
		case 13:
			ostr << "King of Spades" << endl;
			return ostr;
		case 12:
			ostr << "Queen of Spades" << endl;
			return ostr;
		case 11:
			ostr << "Jack of Spades" << endl;
			return ostr;
		default:
			ostr << Card.value << " of Spades" << endl;
			return ostr;
		}
	}
}

class deck  //creates deck class
{
public:
	deck(); //constructor
	friend ostream &operator<< (ostream &ostr, const deck &Deck); //overloaded string operator
private:
	node<card> *CardPtr; //card node pointer
};

deck::deck() // deck constructor
{
	node<card> *newCard; //card pointer
	newCard = new node<card>(card(2, 0), NULL);  //points to the first card
	CardPtr = newCard; //sets cardpointer to newcard

	for (int i = 0; i < 4; i++)
	{
		for (int j = 2; j < 15; j++)
		{
			if (i == 0 && j == 2) continue;

			node<card> *temp = new node<card>(card(j, i), NULL);  // temporary pointer sets card value and suit to numbers for value and suit

			newCard->next = temp;// sets newcard pointer .next to temp
			newCard = temp;// sets newcard to temp
		}
	}
}
//}


ostream &operator<< (ostream &ostr, const deck &Deck) // overloaded print operator
{
	node<card> *CurrentCard = Deck.CardPtr;

	while (CurrentCard != NULL)// while the deck card pointer is not null
	{
		ostr << CurrentCard->nodeValue << endl; //return nodeValue of current card
		CurrentCard = CurrentCard->next; //current card set to next card
	}

	return ostr;
}

void main() // runs program
{

	try // creates a deck and prints it if there is no range errors.
	{	
	deck deck1;
	cout << deck1;
	}

	catch (rangeError &rangeerr)
	{
		cout << rangeerr.what() << endl;
	}

	_getch();//pause to view the deck
}
