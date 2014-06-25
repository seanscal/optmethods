#include "d_except.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

/* Enumerated 'suit' data type 
   Needs too be global so deck can access
*/
enum Suit { CLUBS, DIAMONDS, HEARTS, SPADES };

/* card class */
class card
{
public:
        card();                                                                        // default constructor
        card(const int, const Suit);                        // constructor
        int getValue() const { return value; }        // returns card value
        void setValue(int);                                                // sets card value
        Suit getSuit() const { return suit; }        // returns card suit
        void setSuit(const Suit);                                // sets card suit

        friend ostream &operator<<(ostream &out, const card &c);

private:
        int value;
        Suit suit;
};

/* default card Constructor */
card::card()
{}

/* card Constructor */
card::card(const int val, const Suit s)
        : value(val), suit(s)
{
        // check inputs
        if (val < 2 || val > 14)
        {
                throw rangeError("ERROR: Invalid card value");
        }
        if (s < 0 || s > 3)
        {
                throw rangeError("ERROR: Invalid suit name");
        }
}

/* Sets card value */
void card::setValue(int val)
{
        // check input
        if (val < 2 || val > 14)
        {
                throw rangeError("ERROR: Invalid card value");
        }

        value = val;
} // end setValue

/* Sets suit */
void card::setSuit(const Suit s)
{
        // check input
        if (s < 0 || s > 3)
        {
                throw rangeError("ERROR: Invalid suit name");
        }

        suit = s;
} // end setSuit

// strings that we will call to output the card values
// string of suits 
const string suitStr[] = { "Clubs", "Diamonds", "Hearts", "Spades" };
// string of face cards
const string faceCard[] = { "Jack", "Queen", "King", "Ace" };

/* Overload the '<<' operator for card objects */
ostream &operator<<(ostream &ostr, const card &c)
{
        int cardVal = c.getValue();

        // if the card is less than 11 it is a number and we can output it
        if (cardVal < 11)
        {
                ostr << cardVal << " of " << suitStr[c.getSuit()];
        }
        // if the card is greater than 11 it is a face card and we must
        // subtract 11 from that number so we can find the face card name
        else
        {
                ostr << faceCard[cardVal - 11] << " of " << suitStr[c.getSuit()];
        }

        return ostr;
}