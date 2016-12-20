#include "program.h"
#include <iostream>
#include <algorithm>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#ifndef deck_h
#define deck_h

using namespace std;

class Hand;
class Deck{
	friend ostream & operator<<(ostream & os, const Deck &deck);
	friend int main(int argc, char * argv[]);
	friend void operator<<( Hand &h, Deck &d);
public:
	Deck();
	int Deck::add_card(Card card);
	int Deck::shuffleCards();
	int size();
	Card removeCard();
	//I don't need a destructor because we don't call new so I wouldn't generate memory leaks
private: 
	 vector<Card> deck;
};



#endif