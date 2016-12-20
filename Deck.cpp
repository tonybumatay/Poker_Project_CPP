#include "stdafx.h"
#include "Deck.h"
#include "stdlib.h"
#include <time.h>
#include <random>
#include <chrono>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

Deck::Deck(){
	deck = {};
}

Card Deck::removeCard(){
	Card temp = deck[deck.size() - 1];
	deck.pop_back();
	return temp;
}

int Deck::size(){
	return deck.size();
}

int Deck::add_card(Card card){
	deck.push_back(card);
	return returnVals::success;
}

int deckSize(const vector<Card> deck){//Is this what they want for a size method
	return deck.size();
}

int Deck::shuffleCards(){//getting here, but not actually changing the order
	srand(time(0));
	random_shuffle(deck.begin(), deck.end());
	return returnVals::success;
}

ostream & operator<<(ostream & os, const Deck &d){//A non-member insertion operator (operator<<) that takes a reference to an ostream and a reference to a const deck object, and uses the passed ostream to print out valid card definition strings on separate lines, for each card in the deck object's container member variable. Note that your implementation may use a friend declaration to grant this operator access to the deck object's private container member variable.
	const string rankConvert[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};//figure out how to make friends work correctly
	const string suitConvert[] = {"C", "D", "H", "S"};
		//use ostream to print out all of the valid cards in the deck vector
	for (unsigned int i = 0; i < d.deck.size(); ++i){
		cout << rankConvert[d.deck[i].rank+2] << suitConvert[d.deck[i].suit] << endl; //print each valid card on a new line
	}
	return os;
}