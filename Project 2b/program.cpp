//Used Microsoft Visual Studio 2013, program is functional in this dev environment.
//May need small syntax changes to accomodate any version changes since lab version of Visual Studio (ex. _getch() to getch())

#include "stdafx.h"
#include "d_except.h"
#include "d_node.h"
#include <string>
#include <vector>
#include "iostream"
#include "conio.h"
#include <random>
#include <algorithm>
#include <iterator>
#include <time.h>

using namespace std;

class card
{
public:
	card(int n, int m);  //constructor. n = value, m = suit
	card(const card &anothercard);//copy constructor
	void setvalue(int val);
	void setsuit(int newsuit);
	const int getvalue();
	const int getsuit();
	friend ostream &operator<< (ostream &ostr, const card &Card);

private:
	int value;
	int Suit;
};

card::card(const card &anothercard)//Copy Constructor
: value(anothercard.value), Suit(anothercard.Suit)
{}

card::card(int n, int m)// constructor, sets value and Suit to n and m, checks for exceptions
: value(n), Suit(m)
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
			ostr << "Ace of Clubs";
			return ostr;
		case 13:
			ostr << "King of Clubs";
			return ostr;
		case 12:
			ostr << "Queen of Clubs";
			return ostr;
		case 11:
			ostr << "Jack of Clubs";
			return ostr;
		default:
			ostr << Card.value << " of Clubs";
			return ostr;
		}
	case 1:
		switch (Card.value){
		case 14:
			ostr << "Ace of Hearts";
			return ostr;
		case 13:
			ostr << "King of Hearts";
			return ostr;
		case 12:
			ostr << "Queen of Hearts";
			return ostr;
		case 11:
			ostr << "Jack of Hearts";
			return ostr;
		default:
			ostr << Card.value << " of Hearts" ;
			return ostr;
		}
	case 2:
		switch (Card.value){
		case 14:
			ostr << "Ace of Diamonds";
			return ostr;
		case 13:
			ostr << "King of Diamonds";
			return ostr;
		case 12:
			ostr << "Queen of Diamonds";
			return ostr;
		case 11:
			ostr << "Jack of Diamonds";
			return ostr;
		default:
			ostr << Card.value << " of Diamonds";
			return ostr;
		}
	case 3:
		switch (Card.value){
		case 14:
			ostr << "Ace of Spades";
			return ostr;
		case 13:
			ostr << "King of Spades";
			return ostr;
		case 12:
			ostr << "Queen of Spades";
			return ostr;
		case 11:
			ostr << "Jack of Spades";
			return ostr;
		default:
			ostr << Card.value << " of Spades";
			return ostr;
		}
	}
}

class deck  //creates deck class
{
public:
	deck(); //constructor
	deck::deck(const deck &anotherdeck); // copy constructor
	deck::~deck();//destructor
	card deal();
	void deck::replace(card BottomCard);
	void deck::shuffle();
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

card deck::deal()// deals the top card and deletes it from the deck
{
	if (CardPtr != NULL)
	{
		node<card> *TopCard = CardPtr;
		card value = TopCard->nodeValue;
		CardPtr = CardPtr->next;
		delete TopCard;
		return value;
	}
	else // error for no cards
	{
		throw underflowError("ERROR: No cards to deal");
	}
}

deck::deck(const deck &anotherdeck) //Copy Constructer
:CardPtr(anotherdeck.CardPtr)
{}

deck::~deck()//destructor
{
	node<card> *nextcard = NULL;
	while (CardPtr != NULL)
	{
		nextcard = CardPtr->next;
		delete CardPtr;
		CardPtr = nextcard;
	}
}

void deck::replace(card BottomCard)//Bottom Card is passed as the card which was just dealt
{
	node<card> *temp = CardPtr;//avoids changing CardPtr, creates a temporary pointer instead
	node<card> *bottom = NULL;// creates a bottom card, sets it to nell temporarily
	int count = 0;//helps check number of cards in deck

	while (temp != NULL)//sets bottom = to temp when the next card is null, or when there is no next card, at the bottom of the deck
	{
		if (temp->next == NULL)
		{
			bottom = temp;
		} 
		temp = temp->next;
		count++;
	}
	if (count >= 52)//throws an error if there ar 52 cards already
	{
		throw overflowError("ERROR: Deck is already full");
	}

	temp = new node<card>(BottomCard, NULL);//resets temp to the bottom card
	bottom->next = temp;//the next bottom card becomes the card which was just dealt, while the current was created here
}

void deck::shuffle()//creates a vector to store the cards and then used the shuffle function to randomize them
{					// this process ensures that there are no duplicates in the deck when shuffled
	vector<card> deckVector;
	node<card> *SingleCard = CardPtr;
	while (SingleCard != NULL)//creation of a card vector, puts the deck in card by card to create a 52 element vector
	{
		deckVector.push_back(SingleCard->nodeValue);
		SingleCard = SingleCard->next;
	}

	srand(time(NULL));//makes sure that the shuffle is different each time it is run
	// uses shuffle function within std to shuffle all of the elements in the vector to random positions
	random_shuffle(deckVector.begin(), deckVector.end());

	// transfer cards back to a list
	int cardNum = 0;
	node<card> *top = CardPtr;

	while (CardPtr != NULL)//puts the vector back into the deck card by card until it is full.
	{
		CardPtr->nodeValue = deckVector[cardNum];
		CardPtr = CardPtr->next;
		cardNum++;
	}

	CardPtr = top;// cardptr set to original value
}

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

void playFlip()
{

	int points = 0;
	int total = 0;
	int count = 0;
	deck newdeck;
	newdeck.shuffle();//randomizes the deck

	cout << "Press the space bar to deal a card, press any other key twice to stop playing" << endl<<endl;
	while (_getch() == ' ')
	{
		card Card = newdeck.deal();
		
		int val = Card.getvalue();
		int Suit = Card.getsuit();
		cout << Card << endl;

		// if else if to calculate all points
		if (val == 14)
		{
			points += 10;			//10 points for Ace
			if (Suit == 1)			//Heart
			{
				points += 1;
			}
		}
		else if (val == 13 || val == 12 || val == 11)
		{
			points += 5;			//5 points for non-Ace royal
			if (Suit == 1)			//Heart
			{
				points += 1;
			}
		}
		else if (val == 8 || val == 9 || val == 10)
		{
			if (Suit == 1)			//Heart, No points for 8 through 10
			{
				points += 1;
			}
		}
		
		else if (val == 7)  
		{
			points /= 2;			//points cut in half for a 7
			if (Suit == 1)			//Heart
			{
				points += 1;
			}
		}
		else if (val == 2 || val == 3 || val == 4 || val == 5 || val == 6)
		{
			points = 0;				//points reset for 2 through 6 
			if (Suit == 1)			//Heart
			{
				points += 1;
			}
		}

		total++;// total cards
		//displays for end of deck game over or quitting the game early
		cout << "Your score is: " << points << " after " << total<<" cards."<<endl<<endl;
		
		newdeck.replace(Card);//replaces the bottom card, will be shuffled once the deck has gone through all 52 cards
		
		count++;
		if (count == 51) // cycles through the deck until 52 cards are played, then shuffles the deck again
		{
			count = 0;
			newdeck.shuffle();
		}
	}

	if (_getch() != 'p')
		{
		cout << "You chosen to end the game with a score of " << points << endl;
		if (points >= 18)
			cout << "Great Job!" << endl;
		else
			cout << "Try Again, You can do better..." << endl;
		_getch();
		}
}

int main()
{


	// try catch to catch all errors which have been implemented
	try
	{
		playFlip();
	}
	catch (rangeError &rangeerr)
	{
		cout << rangeerr.what() << endl;
	}
	catch (underflowError &undererr)
	{
		cout << undererr.what() << endl;
	}
	catch (overflowError &overerr)
	{
		cout << overerr.what() << endl;
	}
} 
