#include "Deck.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#ifndef hand_h
#define hand_h

using namespace std;
int pokerScore(const Hand &temp);
void operator<<(Hand &h, Deck &d);
string asString(const Hand &h);
bool poker_rank(const Hand &h1, const Hand &h2);
class Deck;
class Hand{
	friend ostream & operator<<(ostream & os, const Hand &hand);//figure out how to make friends work
	friend void operator<<( Hand &h, Deck &d);
	friend void operator<<(Hand &h1, Hand &h2);
	friend void operator<<(Hand &h, Card &c);
	friend bool operator==(const Hand & h1, const Hand & h2);
	friend string asString(const Hand &h);
	friend bool poker_rank(const Hand &h1, const Hand &h2);
	friend int pokerScore(const Hand &temp);
	friend int main(int argc, char * argv[]);
	
public:
	Hand(){//Constructor
	}
	//I don't need a destructor because we don't call new so I wouldn't generate memory leaks
	void Hand::operator=(const Hand &second);
	bool operator<(const Hand& h) const;
	bool poker_rank(const Hand &h1, const Hand &h2);
	Card operator[](size_t index);
	int remove_card(size_t index);
	int size();
private:
	vector<Card> hand;
		
};



struct scoreName{
	enum score{ noRank = 0, onePair, twoPair, threeOfAKind, straight, flush, fullHouse, fourOfAKind, straightFlush };
};

struct matchedCards{
	enum numMatched{zero, one, two, three, four};
};
struct cardNum{
	enum cardNumber{firstCard, secondCard, thirdCard, fourthCard, fifthCard, numCards};
};
#endif