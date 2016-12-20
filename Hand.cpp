#include "stdafx.h"
#include "Hand.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>

using namespace std;
Card Hand::operator[](size_t index){
	if (index > hand.size() - 1){
		throw int(returnVals::errorHandNum);
	}
	else{
		return hand[index];
	}
}

int Hand::size(){
	return hand.size();
}

int Hand::remove_card(size_t index){
	if (index > hand.size()-1){
		throw int(returnVals::errorHandNum);
	}
	else{
		hand.erase(hand.begin() + index);
		return returnVals::success;
	}
}

//assumes hands are already ordered by rank
bool Hand::operator<(const Hand& h) const{//Compare the first card in each hand and if they are the same compare the second card, etc. the ordering of the first cards that differ determines the ordering. Also ex. 9H is less than 9H 10H
	int smallerHand;
	bool smaller;
	if (h.hand.size() <= hand.size()){
		smallerHand = h.hand.size();
		smaller = false;
	}
	if (h.hand.size() > hand.size()){
		smallerHand = hand.size();
		smaller = true;
	}
	for (int i = 0; i < smallerHand; ++i){
		if (hand[i]< h.hand[i]){
			return true;//if the ith card in hand < ith card in h.hand 
		}
		if (h.hand[i]<hand[i]){
			return false;
		}
	}
	if (smaller){
		return true;//if two hands are equal until one ends, and the shorter on is hand
	}
	return false;//if two hands are equal until one ends, and the shorter on is h.hand
}

void Hand::operator=(const Hand &second){
	if (this == &second){//nothing, cuz we already guchi
	}
	else{
		hand.clear();
		for (unsigned int i = 0; i < second.hand.size(); ++i){
			hand.push_back(second.hand[i]);
		}//now we guchi, cuz we just copied the hand vector
	}
};



int handSize(const vector<Card> hand){//Is this what they want for a size method
	return hand.size();
}

bool operator==(const Hand & h1, const Hand & h2){//const equivalence operator compares two hands
	bool sameSize = false;
	if (h1.hand.size()== h2.hand.size()){//check if hands are equal in size
		sameSize= true;
	}
	if (sameSize == false){//if the hands are not the same size
		return false;
	}
	for (unsigned int i = 0; i < h1.hand.size(); ++i){//check each card in the hand
		if ((h1.hand[i].rank == h2.hand[i].rank) && (h1.hand[i].suit == h2.hand[i].suit)){//if the cards are equal, do nothing
		}
		else{
			return false;
		}
	}
	return true;
}

void operator<<(Hand &h, Deck &d){//takes elements off the back of a deck and inserts them into a hand
	h.hand.push_back(d.deck.back());
	d.deck.pop_back();
	sort(h.hand.begin(), h.hand.end());
}
void operator<<(Hand &h1, Hand &h2){//takes elements off the back of a hand and inserts them into a hand
	for (size_t i = 0; i < h2.size(); ++i){
		h1.hand.push_back(h2[i]);
	}
	sort(h1.hand.begin(), h1.hand.end());
}
void operator<<(Hand &h, Card &c){//takes a card and inserts it into a hand
	h.hand.push_back(c);
	sort(h.hand.begin(), h.hand.end());
}
ostream & operator<<(ostream & os, const Hand &h){//A non-member insertion operator (operator<<) that takes a reference to an ostream and a reference to a const deck object, and uses the passed ostream to print out valid card definition strings on separate lines, for each card in the deck object's container member variable. Note that your implementation may use a friend declaration to grant this operator access to the deck object's private container member variable.
	const string rankConvert[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };//figure out how to make friends work correctly
	const string suitConvert[] = { "C", "D", "H", "S" };
	//use ostream to print out all of the valid cards in the hand vector
	for (unsigned int i = 0; i < h.hand.size(); ++i){
		cout << rankConvert[h.hand[i].rank+2] << suitConvert[h.hand[i].suit] << " "; //print each valid card on a new line
	}
	cout << "" << endl;
	return os;
}

string asString(const Hand &h){
	const string rankConvert[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A" };//figure out how to make friends work correctly
	const string suitConvert[] = { "C", "D", "H", "S" };
	string output; 
	//use ostream to print out all of the valid cards in the deck vector
	for (unsigned int i = 0; i < h.hand.size(); ++i){
		output= output + " " + rankConvert[h.hand[i].rank+2] + "" + suitConvert[h.hand[i].suit]; //print each valid card on a new line
	}
	return output;
}


bool poker_rank(const Hand &h1, const Hand &h2){//return true if h1 ranks higher than h2
	int h1Score=0;
	int h2Score=0;
	//First Hand
	vector<vector<Card>> rankCounth1(cardNum::numCards);//vector of vector of cards
	rankCounth1[cardNum::fifthCard].push_back (h1.hand[cardNum::fifthCard]);//highest card is always highest

	for (int i = (h1.hand.size() - 2); i >= 0; --i){//JK don't harcode constants
		if (h1.hand[i].rank == h1.hand[cardNum::fifthCard].rank){//is second highest the same rank as the highest?
			rankCounth1[cardNum::fifthCard].push_back(h1.hand[i]);
		}
		else if (h1.hand[i].rank == h1.hand[cardNum::fourthCard].rank){
			rankCounth1[cardNum::fourthCard].push_back(h1.hand[i]);
		}
		else if (h1.hand[i].rank == h1.hand[cardNum::thirdCard].rank){
			rankCounth1[cardNum::thirdCard].push_back(h1.hand[i]);
		}
		else if (h1.hand[i].rank == h1.hand[cardNum::secondCard].rank){
			rankCounth1[cardNum::secondCard].push_back(h1.hand[i]);
		}
		else if (h1.hand[i].rank == h1.hand[cardNum::firstCard].rank){
			rankCounth1[cardNum::firstCard].push_back(h1.hand[i]);
		}
	}
	
	//straight flush: the 5 cards are consecutive in rank and have the same suit
	if ((h1.hand[cardNum::firstCard].suit == h1.hand[cardNum::secondCard].suit) && (h1.hand[cardNum::firstCard].suit == h1.hand[cardNum::thirdCard].suit) && (h1.hand[cardNum::firstCard].suit == h1.hand[cardNum::fourthCard].suit) && (h1.hand[cardNum::firstCard].suit == h1.hand[cardNum::fifthCard].suit)){//all cards are the same suit
		if (((h1.hand[cardNum::firstCard].rank == (h1.hand[cardNum::secondCard].rank - 1)) && (h1.hand[cardNum::firstCard].rank == (h1.hand[cardNum::thirdCard].rank - 2)) && (h1.hand[cardNum::firstCard].rank == (h1.hand[cardNum::fourthCard].rank - 3)) && (h1.hand[cardNum::firstCard].rank == (h1.hand[cardNum::fifthCard].rank - 4))) ||//straight flush: the 5 cards are consecutive in rank and have the same suit
			((h1.hand[cardNum::fifthCard].rank == 12) && (h1.hand[cardNum::firstCard].rank == 0) && (h1.hand[cardNum::secondCard].rank == 1) && (h1.hand[cardNum::thirdCard].rank == 2) && (h1.hand[cardNum::fourthCard].rank == 3))){//edge case when Ace is the low end of a straight
			h1Score= scoreName::straightFlush;
		}
		else{//flush: the 5 card are not all consecutive in rank, but have the same suit
			h1Score = scoreName::flush;
		}
	}
	//four of a kind: 4 of the 5 cards have the same rank
	else if ((rankCounth1[cardNum::firstCard].size() == matchedCards::four) || (rankCounth1[cardNum::secondCard].size() == matchedCards::four) || (rankCounth1[cardNum::thirdCard].size() == matchedCards::four) || (rankCounth1[cardNum::fourthCard].size() == matchedCards::four) || (rankCounth1[cardNum::fifthCard].size() == matchedCards::four)){
		h1Score = scoreName::fourOfAKind;
	}
	//full house: 3 of the 5 cards have the same rank and the other two have the same rank
	else if (((rankCounth1[cardNum::firstCard].size() == matchedCards::three) || (rankCounth1[cardNum::secondCard].size() == matchedCards::three) || (rankCounth1[cardNum::thirdCard].size() == matchedCards::three) || (rankCounth1[cardNum::fourthCard].size() == matchedCards::three) || (rankCounth1[cardNum::fifthCard].size() == matchedCards::three)) && ((rankCounth1[cardNum::firstCard].size() == matchedCards::two) || (rankCounth1[cardNum::secondCard].size() == matchedCards::two) || (rankCounth1[cardNum::thirdCard].size() == matchedCards::two) || (rankCounth1[cardNum::fourthCard].size() == matchedCards::two) || (rankCounth1[cardNum::fifthCard].size() == matchedCards::two))){
		h1Score = scoreName::fullHouse;
	}

	//straight: the 5 cards are all consecutive in rank but do not all have the same suit
	else if (((h1.hand[cardNum::firstCard].rank == (h1.hand[cardNum::secondCard].rank - 1)) && (h1.hand[cardNum::firstCard].rank == (h1.hand[cardNum::thirdCard].rank - 2)) && (h1.hand[cardNum::firstCard].rank == (h1.hand[3].rank - 3)) && (h1.hand[cardNum::firstCard].rank == (h1.hand[cardNum::fifthCard].rank - 4))) ||
		((h1.hand[cardNum::fifthCard].rank == 12) && (h1.hand[cardNum::firstCard].rank == 0) && (h1.hand[cardNum::secondCard].rank == 1) && (h1.hand[cardNum::thirdCard].rank == 2) && (h1.hand[3].rank == 3))){//edge case when Ace is low
		h1Score = scoreName::straight;
	}
	//three of a kind: 3 of the 5 cards have the same rank and the other 2 cards have different ranks
	else if (((rankCounth1[cardNum::firstCard].size() == matchedCards::three) || (rankCounth1[cardNum::secondCard].size() == matchedCards::three) || (rankCounth1[cardNum::thirdCard].size() == matchedCards::three) || (rankCounth1[cardNum::fourthCard].size() == matchedCards::three) || (rankCounth1[cardNum::fifthCard].size() == matchedCards::three))
		&& ((rankCounth1[cardNum::firstCard].size() == matchedCards::one) || (rankCounth1[cardNum::secondCard].size() == matchedCards::one) || (rankCounth1[cardNum::thirdCard].size() == matchedCards::one) || (rankCounth1[cardNum::fourthCard].size() == matchedCards::one) || (rankCounth1[cardNum::fifthCard].size() == matchedCards::one))){
		h1Score = scoreName::threeOfAKind;
	}
	//two pairs: 2 of the 5 cards have the same rank, another of the 2 cards are of the same ranks, and the last is of different rank
	else if (((rankCounth1[cardNum::firstCard].size() == matchedCards::two) && (rankCounth1[cardNum::secondCard].size() == matchedCards::two)) || ((rankCounth1[cardNum::firstCard].size() == matchedCards::two) && (rankCounth1[cardNum::thirdCard].size() == matchedCards::two)) || ((rankCounth1[cardNum::firstCard].size() == matchedCards::two) && (rankCounth1[cardNum::fourthCard].size() == matchedCards::two)) ||
		((rankCounth1[cardNum::firstCard].size() == matchedCards::two) && (rankCounth1[cardNum::fifthCard].size() == matchedCards::two)) || ((rankCounth1[cardNum::secondCard].size() == matchedCards::two) && (rankCounth1[cardNum::thirdCard].size() == matchedCards::two)) || ((rankCounth1[cardNum::secondCard].size() == matchedCards::two) && (rankCounth1[cardNum::fourthCard].size() == matchedCards::two)) ||
		((rankCounth1[cardNum::secondCard].size() == matchedCards::two) && (rankCounth1[cardNum::fifthCard].size() == matchedCards::two)) || ((rankCounth1[cardNum::thirdCard].size() == matchedCards::two) && (rankCounth1[cardNum::fourthCard].size() == matchedCards::two)) || ((rankCounth1[cardNum::thirdCard].size() == matchedCards::two) && (rankCounth1[cardNum::fifthCard].size() == matchedCards::two)) ||
		((rankCounth1[cardNum::fourthCard].size() == matchedCards::two) && (rankCounth1[cardNum::fifthCard].size() == matchedCards::two))){
		h1Score = scoreName::twoPair;
	}
	//one pair: 2 of the 5 cards have the same rank and the other 3 have different ranks
	else if (((rankCounth1[cardNum::firstCard].size() == matchedCards::two) || (rankCounth1[cardNum::secondCard].size() == matchedCards::two) || (rankCounth1[cardNum::thirdCard].size() == matchedCards::two) || (rankCounth1[cardNum::fourthCard].size() == matchedCards::two) || (rankCounth1[cardNum::fifthCard].size() == matchedCards::two))
		&& ((rankCounth1[cardNum::firstCard].size() == matchedCards::one) || (rankCounth1[cardNum::secondCard].size() == matchedCards::one) || (rankCounth1[cardNum::thirdCard].size() == matchedCards::one) || (rankCounth1[cardNum::fourthCard].size() == matchedCards::one) || (rankCounth1[cardNum::fifthCard].size() == matchedCards::one))){
		h1Score = scoreName::onePair;
	}
	//no rank: none of the other ranks apply
	else{
		h1Score = scoreName::noRank;
	}
	
	//Second Hand

	vector<vector<Card>> rankCounth2(cardNum::numCards);//vector of vector of cards
	rankCounth2[cardNum::fifthCard].push_back(h2.hand[cardNum::fifthCard]);//highest card is always highest
	for (int i = (h2.hand.size() - 2); i >= 0; --i){//JK don't harcode constants
		if (h2.hand[i].rank == h2.hand[cardNum::fifthCard].rank){//is second highest the same rank as the highest?
			rankCounth2[cardNum::fifthCard].push_back(h2.hand[i]);
		}
		else if (h2.hand[i].rank == h2.hand[cardNum::fourthCard].rank){
			rankCounth2[cardNum::fourthCard].push_back(h2.hand[i]);
		}
		else if (h2.hand[i].rank == h2.hand[cardNum::thirdCard].rank){
			rankCounth2[cardNum::thirdCard].push_back(h2.hand[i]);
		}
		else if (h2.hand[i].rank == h2.hand[cardNum::secondCard].rank){
			rankCounth2[cardNum::secondCard].push_back(h2.hand[i]);
		}
		else if (h2.hand[i].rank == h2.hand[cardNum::firstCard].rank){
			rankCounth2[cardNum::firstCard].push_back(h2.hand[i]);
		}
	}
	
	//straight flush: the 5 cards are consecutive in rank and have the same suit
	if ((h2.hand[cardNum::firstCard].suit == h2.hand[cardNum::secondCard].suit) && (h2.hand[cardNum::firstCard].suit == h2.hand[cardNum::thirdCard].suit) && (h2.hand[cardNum::firstCard].suit == h2.hand[cardNum::fourthCard].suit) && (h2.hand[cardNum::firstCard].suit == h2.hand[cardNum::fifthCard].suit)){//all cards are the same suit
		if (((h2.hand[cardNum::firstCard].rank == (h2.hand[cardNum::secondCard].rank - 1)) && (h2.hand[cardNum::firstCard].rank == (h2.hand[cardNum::thirdCard].rank - 2)) && (h2.hand[cardNum::firstCard].rank == (h2.hand[cardNum::fourthCard].rank - 3)) && (h2.hand[cardNum::firstCard].rank == (h2.hand[cardNum::fifthCard].rank - 4))) ||//straight flush: the 5 cards are consecutive in rank and have the same suit
			((h2.hand[cardNum::fifthCard].rank == 12) && (h2.hand[cardNum::firstCard].rank == 0) && (h2.hand[cardNum::secondCard].rank == 1) && (h2.hand[cardNum::thirdCard].rank == 2) && (h2.hand[cardNum::fourthCard].rank == 3))){//edge case when Ace is the low end of a straight
			h2Score = scoreName::straightFlush;
		}
		else{//flush: the 5 card are not all consecutive in rank, but have the same suit
			h2Score = scoreName::flush;
		}
	}
	//four of a kind: 4 of the 5 cards have the same rank
	else if ((rankCounth2[cardNum::firstCard].size() == matchedCards::four) || (rankCounth2[cardNum::secondCard].size() == matchedCards::four) || (rankCounth2[cardNum::thirdCard].size() == matchedCards::four) || (rankCounth2[cardNum::fourthCard].size() == matchedCards::four) || (rankCounth2[cardNum::fifthCard].size() == matchedCards::four)){
		h2Score = scoreName::fourOfAKind;
	}
	//full house: 3 of the 5 cards have the same rank and the other two have the same rank
	else if (((rankCounth2[cardNum::firstCard].size() == matchedCards::three) || (rankCounth2[cardNum::secondCard].size() == matchedCards::three) || (rankCounth2[cardNum::thirdCard].size() == matchedCards::three) || (rankCounth2[cardNum::fourthCard].size() == matchedCards::three) || (rankCounth2[cardNum::fifthCard].size() == matchedCards::three)) && ((rankCounth2[cardNum::firstCard].size() == matchedCards::two) || (rankCounth2[cardNum::secondCard].size() == matchedCards::two) || (rankCounth2[cardNum::thirdCard].size() == matchedCards::two) || (rankCounth2[cardNum::fourthCard].size() == matchedCards::two) || (rankCounth2[cardNum::fifthCard].size() == matchedCards::two))){
		h2Score = scoreName::fullHouse;
	}


	//straight: the 5 cards are all consecutive in rank but do not all have the same suit
	else if (((h2.hand[cardNum::firstCard].rank == (h2.hand[cardNum::secondCard].rank - 1)) && (h2.hand[cardNum::firstCard].rank == (h2.hand[cardNum::thirdCard].rank - 2)) && (h2.hand[cardNum::firstCard].rank == (h2.hand[3].rank - 3)) && (h2.hand[cardNum::firstCard].rank == (h2.hand[cardNum::fifthCard].rank - 4))) ||
		((h2.hand[cardNum::fifthCard].rank == 12) && (h2.hand[cardNum::firstCard].rank == 0) && (h2.hand[cardNum::secondCard].rank == 1) && (h2.hand[cardNum::thirdCard].rank == 2) && (h2.hand[3].rank == 3))){//edge case when Ace is low
		h2Score = scoreName::straight;
	}
	//three of a kind: 3 of the 5 cards have the same rank and the other 2 cards have different ranks
	else if (((rankCounth2[cardNum::firstCard].size() == matchedCards::three) || (rankCounth2[cardNum::secondCard].size() == matchedCards::three) || (rankCounth2[cardNum::thirdCard].size() == matchedCards::three) || (rankCounth2[cardNum::fourthCard].size() == matchedCards::three) || (rankCounth2[cardNum::fifthCard].size() == matchedCards::three))
		&& ((rankCounth2[cardNum::firstCard].size() == matchedCards::one) || (rankCounth2[cardNum::secondCard].size() == matchedCards::one) || (rankCounth2[cardNum::thirdCard].size() == matchedCards::one) || (rankCounth2[cardNum::fourthCard].size() == matchedCards::one) || (rankCounth2[cardNum::fifthCard].size() == matchedCards::one))){
		h2Score = scoreName::threeOfAKind;
	}
	//two pairs: 2 of the 5 cards have the same rank, another of the 2 cards are of the same ranks, and the last is of different rank
	else if (((rankCounth2[cardNum::firstCard].size() == matchedCards::two) && (rankCounth2[cardNum::secondCard].size() == matchedCards::two)) || ((rankCounth2[cardNum::firstCard].size() == matchedCards::two) && (rankCounth2[cardNum::thirdCard].size() == matchedCards::two)) || ((rankCounth2[cardNum::firstCard].size() == matchedCards::two) && (rankCounth2[cardNum::fourthCard].size() == matchedCards::two)) ||
		((rankCounth2[cardNum::firstCard].size() == matchedCards::two) && (rankCounth2[cardNum::fifthCard].size() == matchedCards::two)) || ((rankCounth2[cardNum::secondCard].size() == matchedCards::two) && (rankCounth2[cardNum::thirdCard].size() == matchedCards::two)) || ((rankCounth2[cardNum::secondCard].size() == matchedCards::two) && (rankCounth2[cardNum::fourthCard].size() == matchedCards::two)) ||
		((rankCounth2[cardNum::secondCard].size() == matchedCards::two) && (rankCounth2[cardNum::fifthCard].size() == matchedCards::two)) || ((rankCounth2[cardNum::thirdCard].size() == matchedCards::two) && (rankCounth2[cardNum::fourthCard].size() == matchedCards::two)) || ((rankCounth2[cardNum::thirdCard].size() == matchedCards::two) && (rankCounth2[cardNum::fifthCard].size() == matchedCards::two)) ||
		((rankCounth2[cardNum::fourthCard].size() == matchedCards::two) && (rankCounth2[cardNum::fifthCard].size() == matchedCards::two))){
		h2Score = scoreName::twoPair;
	}
	//one pair: 2 of the 5 cards have the same rank and the other 3 have different ranks
	else if (((rankCounth2[cardNum::firstCard].size() == matchedCards::two) || (rankCounth2[cardNum::secondCard].size() == matchedCards::two) || (rankCounth2[cardNum::thirdCard].size() == matchedCards::two) || (rankCounth2[cardNum::fourthCard].size() == matchedCards::two) || (rankCounth2[cardNum::fifthCard].size() == matchedCards::two))
		&& ((rankCounth2[cardNum::firstCard].size() == matchedCards::one) || (rankCounth2[cardNum::secondCard].size() == matchedCards::one) || (rankCounth2[cardNum::thirdCard].size() == matchedCards::one) || (rankCounth2[cardNum::fourthCard].size() == matchedCards::one) || (rankCounth2[cardNum::fifthCard].size() == matchedCards::one))){
		h2Score = scoreName::onePair;
	}
	//no rank: none of the other ranks apply
	else{
		h2Score = scoreName::noRank;
	}

	if (h1Score > h2Score){//h1 has a higher poker score
		return true;
	}
	else if (h2Score > h1Score){//h2 has a higher poker score
		return false;
	}

	else{//h1Score=h2Score
		
		if (h1Score == scoreName::straightFlush){//FOR STRIAGHT FLUSH
			if (h1.hand[4].rank > h2.hand[4].rank){//h1 rank is higher
				return true;
			}
			else if (h1.hand[4].rank < h2.hand[4].rank){//h2 rank is higher
				return false;
			}
			else{
				if (h1.hand[4].suit>h2.hand[4].suit){//h1 suit is higher
					return true;
				}
				else{//h1 must not have a greater poker score than h2
					return false;
				}
			}
		}

		//Four of a Kind
		if (h1Score == scoreName::fourOfAKind){
			if ((rankCounth1[cardNum::fifthCard].size() == rankCounth2[cardNum::fifthCard].size()) && (rankCounth1[cardNum::fifthCard].size() == matchedCards::four)){//compare h1[4] and h2[4]
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fifthCard].rank){
					return true;
				}
			}
			if ((rankCounth1[cardNum::fourthCard].size() == rankCounth2[cardNum::fourthCard].size()) && (rankCounth1[cardNum::fourthCard].size() == matchedCards::four)){//compare h1[3] and h2[3]
				if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::fourthCard].rank){
					return true;
				}
			}
			if ((rankCounth1[cardNum::fifthCard].size() == rankCounth2[cardNum::fourthCard].size()) && (rankCounth1[cardNum::fifthCard].size() == matchedCards::four)){//compare h1[4] and h2[3]
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fourthCard].rank){
					return true;
				}
			}
			if ((rankCounth1[cardNum::fourthCard].size() == rankCounth2[cardNum::fifthCard].size()) && (rankCounth1[cardNum::fourthCard].size() == matchedCards::four)){//compare h1[3] and h2[4]
				if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::fifthCard].rank){
					return true;
				}
			}
			return false;//h2 has a higher poker score
		}

		//Full House
		if (h1Score == scoreName::fullHouse){//no two full houses can have the same 3 of a kind in a 52 card deck
			if ((rankCounth1[cardNum::fifthCard].size() == rankCounth2[cardNum::fifthCard].size()) && (rankCounth1[cardNum::fifthCard].size() == matchedCards::three)){//compare h1[4] and h2[4]
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fifthCard].rank){
					return true;
				}
			}
			if ((rankCounth1[cardNum::fourthCard].size() == rankCounth2[cardNum::fourthCard].size()) && (rankCounth1[cardNum::fourthCard].size() == matchedCards::three)){//compare h1[3] and h2[3]
				if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::fourthCard].rank){
					return true;
				}
			}
			if ((rankCounth1[cardNum::fifthCard].size() == rankCounth2[cardNum::fourthCard].size()) && (rankCounth1[cardNum::fifthCard].size() == matchedCards::three)){//compare h1[4] and h2[3]
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fourthCard].rank){
					return true;
				}
			}
			if ((rankCounth1[cardNum::fourthCard].size() == rankCounth2[cardNum::fifthCard].size()) && (rankCounth1[cardNum::fourthCard].size() == matchedCards::three)){//compare h1[3] and h2[4]
				if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::fifthCard].rank){
					return true;
				}
			}
			return false;//h2 has a higher poker score

		}


		//Flush
		if (h1Score == scoreName::flush){
			if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fifthCard].rank){//compare highest card of both hands
				return true;
			}
			if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fifthCard].rank){
				if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::fourthCard].rank){
					return true;
				}
			}
			if ((h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fifthCard].rank) && (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::fourthCard].rank)){
				if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::thirdCard].rank){
					return true;
				}
			}
			if ((h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fifthCard].rank) && (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::fourthCard].rank) && (h1.hand[cardNum::thirdCard].rank == h2.hand[cardNum::thirdCard].rank)){
				if (h1.hand[cardNum::secondCard].rank > h2.hand[cardNum::secondCard].rank){
					return true;
				}
			}
			if ((h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fifthCard].rank) && (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::fourthCard].rank) && (h1.hand[cardNum::thirdCard].rank == h2.hand[cardNum::thirdCard].rank) && (h1.hand[cardNum::secondCard].rank == h2.hand[cardNum::secondCard].rank)){
				if (h1.hand[cardNum::firstCard].rank > h2.hand[cardNum::firstCard].rank){
					return true;
				}
			}
			//if all ranks are equal, rank by suit
			if ((h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fifthCard].rank) && (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::fourthCard].rank) && (h1.hand[cardNum::thirdCard].rank == h2.hand[cardNum::thirdCard].rank) && (h1.hand[cardNum::secondCard].rank == h2.hand[cardNum::secondCard].rank) && (h1.hand[cardNum::firstCard].rank == h2.hand[cardNum::firstCard].rank)){
				if (h1.hand[cardNum::fifthCard].suit > h2.hand[cardNum::fifthCard].suit){
					return true;
				}
			}
			
			return false;//h2 has a higher poker score
		}


		//Straight
		if (h1Score == scoreName::straight){
			if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fifthCard].rank){
				return true;
			}
			else if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fifthCard].rank){//ranks are equal, rank by suit
				if (h1.hand[cardNum::fifthCard].suit > h2.hand[cardNum::fifthCard].suit){
					return true;
				}
			}
			else{//h2 has a higher poker score
				return false;
			}
		}


		//Three of a Kind
		if (h1Score == scoreName::threeOfAKind){
			if ((rankCounth1[cardNum::fifthCard].size() == rankCounth2[cardNum::fifthCard].size()) && (rankCounth1[cardNum::fifthCard].size() == matchedCards::three)){//compare h1[5th] and h2[5th]
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fifthCard].rank){
					return true;
				}
			}
			if ((rankCounth1[cardNum::fifthCard].size() == rankCounth2[cardNum::fourthCard].size()) && (rankCounth1[cardNum::fifthCard].size() == matchedCards::three)){//compare h1[5th] and h2[4th]
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fourthCard].rank){
					return true;
				}
			}
			if ((rankCounth1[cardNum::fifthCard].size() == rankCounth2[cardNum::thirdCard].size()) && (rankCounth1[cardNum::fifthCard].size() == matchedCards::three)){//compare h1[5th] and h2[3rd]
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::thirdCard].rank){
					return true;
				}
			}
			if ((rankCounth1[cardNum::fourthCard].size() == rankCounth2[cardNum::fifthCard].size()) && (rankCounth1[cardNum::fourthCard].size() == matchedCards::three)){//compare h1[4th] and h2[5th]
				if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::fifthCard].rank){
					return true;
				}
			}
			if ((rankCounth1[cardNum::fourthCard].size() == rankCounth2[cardNum::fourthCard].size()) && (rankCounth1[cardNum::fourthCard].size() == matchedCards::three)){//compare h1[4th] and h2[4th]
				if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::fourthCard].rank){
					return true;
				}
			}
			if ((rankCounth1[cardNum::fourthCard].size() == rankCounth2[cardNum::thirdCard].size()) && (rankCounth1[cardNum::fourthCard].size() == matchedCards::three)){//compare h1[4th] and h2[3rd]
				if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::thirdCard].rank){
					return true;
				}
			}
			if ((rankCounth1[cardNum::thirdCard].size() == rankCounth2[cardNum::fifthCard].size()) && (rankCounth1[cardNum::thirdCard].size() == matchedCards::three)){//compare h1[3rd] and h2[5th]
				if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::fifthCard].rank){
					return true;
				}
			}
			if ((rankCounth1[cardNum::thirdCard].size() == rankCounth2[cardNum::fourthCard].size()) && (rankCounth1[cardNum::thirdCard].size() == matchedCards::three)){//compare h1[3rd] and h2[4th]
				if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::fourthCard].rank){
					return true;
				}
			}
			if ((rankCounth1[cardNum::thirdCard].size() == rankCounth2[cardNum::thirdCard].size()) && (rankCounth1[cardNum::thirdCard].size() == matchedCards::three)){//compare h1[3rd] and h2[3rd]
				if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::thirdCard].rank){
					return true;
				}
			}

			return false;

		}
		
		//Two Pair
		if (h1Score == scoreName::twoPair){
			//2 2
			//2 2
			//1 1
			if ((rankCounth1[cardNum::fifthCard].size() == rankCounth2[cardNum::fifthCard].size()) && (rankCounth1[cardNum::fifthCard].size() == matchedCards::two) && ((rankCounth1[cardNum::fourthCard].size() == rankCounth2[cardNum::fourthCard].size()) && (rankCounth1[cardNum::fourthCard].size() == matchedCards::two))){//compare h1[5th] and h2[5th]; h1[4th] and h2[4th]
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fifthCard].rank){//compare the first pair
					return true;
				}
				if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fifthCard].rank){//the first pair is equal
						if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::fourthCard].rank){//compare the second pair
							return true;
						}
						if (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::fourthCard].rank){//if second pair is also equal
							if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::thirdCard].rank){//compare fifth card
								return true;
							}
						}
				}
				return false;
			}

			//2 2
			//2 1
			//1 2
			if ((rankCounth1[cardNum::fifthCard].size() == rankCounth2[cardNum::fifthCard].size()) && (rankCounth1[cardNum::fifthCard].size() == matchedCards::two) && ((rankCounth1[cardNum::fourthCard].size() == rankCounth2[cardNum::thirdCard].size()) && (rankCounth1[cardNum::fourthCard].size() == matchedCards::two))){//compare h1[5th] and h2[5th]; h1[4th] and h2[3rd]
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fifthCard].rank){//compare the first pair
					return true;
				}
				if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fifthCard].rank){//the first pair is equal
					if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::thirdCard].rank){//compare the second pair
						return true;
					}
					if (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::thirdCard].rank){//if second pair is also equal
						if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::fourthCard].rank){//compare fifth card
							return true;
						}
					}
				}
				return false;
			}

			//2 2
			//1 2
			//2 1
			if ((rankCounth1[cardNum::fifthCard].size() == rankCounth2[cardNum::fifthCard].size()) && (rankCounth1[cardNum::fifthCard].size() == matchedCards::two) && ((rankCounth1[cardNum::thirdCard].size() == rankCounth2[cardNum::fourthCard].size()) && (rankCounth1[cardNum::thirdCard].size() == matchedCards::two))){
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fifthCard].rank){//compare the first pair
					return true;
				}
				if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fifthCard].rank){//the first pair is equal
					if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::fourthCard].rank){//compare the second pair
						return true;
					}
					if (h1.hand[cardNum::thirdCard].rank == h2.hand[cardNum::fourthCard].rank){//if second pair is also equal
						if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::thirdCard].rank){//compare fifth card
							return true;
						}
					}
				}
				return false;
			}

			//2 1
			//2 2
			//1 2
			if ((rankCounth1[cardNum::fifthCard].size() == rankCounth2[cardNum::fourthCard].size()) && (rankCounth1[cardNum::fifthCard].size() == matchedCards::two) && ((rankCounth1[cardNum::fourthCard].size() == rankCounth2[cardNum::thirdCard].size()) && (rankCounth1[cardNum::fourthCard].size() == matchedCards::two))){
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fourthCard].rank){//compare the first pair
					return true;
				}
				if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fourthCard].rank){//the first pair is equal
					if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::thirdCard].rank){//compare the second pair
						return true;
					}
					if (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::thirdCard].rank){//if second pair is also equal
						if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::fifthCard].rank){//compare fifth card
							return true;
						}
					}
				}
				return false;
			}

			//2 2
			//1 1
			//2 2

			if ((rankCounth1[cardNum::fifthCard].size() == rankCounth2[cardNum::fifthCard].size()) && (rankCounth1[cardNum::fifthCard].size() == matchedCards::two) && ((rankCounth1[cardNum::thirdCard].size() == rankCounth2[cardNum::thirdCard].size()) && (rankCounth1[cardNum::thirdCard].size() == matchedCards::two))){
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fifthCard].rank){//compare the first pair
					return true;
				}
				if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fifthCard].rank){//the first pair is equal
					if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::thirdCard].rank){//compare the second pair
						return true;
					}
					if (h1.hand[cardNum::thirdCard].rank == h2.hand[cardNum::thirdCard].rank){//if second pair is also equal
						if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::fourthCard].rank){//compare fifth card
							return true;
						}
					}
				}
				return false;
			}

			//2 1
			//1 2
			//2 2

			if ((rankCounth1[cardNum::fifthCard].size() == rankCounth2[cardNum::fourthCard].size()) && (rankCounth1[cardNum::fifthCard].size() == matchedCards::two) && ((rankCounth1[cardNum::thirdCard].size() == rankCounth2[cardNum::thirdCard].size()) && (rankCounth1[cardNum::thirdCard].size() == matchedCards::two))){
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fourthCard].rank){//compare the first pair
					return true;
				}
				if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fourthCard].rank){//the first pair is equal
					if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::thirdCard].rank){//compare the second pair
						return true;
					}
					if (h1.hand[cardNum::thirdCard].rank == h2.hand[cardNum::thirdCard].rank){//if second pair is also equal
						if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::fifthCard].rank){//compare fifth card
							return true;
						}
					}
				}
				return false;
			}

			//1 2
			//2 2
			//2 1

			if ((rankCounth1[cardNum::fourthCard].size() == rankCounth2[cardNum::fifthCard].size()) && (rankCounth1[cardNum::fourthCard].size() == matchedCards::two) && ((rankCounth1[cardNum::thirdCard].size() == rankCounth2[cardNum::fourthCard].size()) && (rankCounth1[cardNum::thirdCard].size() == matchedCards::two))){
				if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::fifthCard].rank){//compare the first pair
					return true;
				}
				if (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::fifthCard].rank){//the first pair is equal
					if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::fourthCard].rank){//compare the second pair
						return true;
					}
					if (h1.hand[cardNum::thirdCard].rank == h2.hand[cardNum::fourthCard].rank){//if second pair is also equal
						if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::thirdCard].rank){//compare fifth card
							return true;
						}
					}
				}
				return false;
			}

			//1 2
			//2 1
			//2 2

			if ((rankCounth1[cardNum::fourthCard].size() == rankCounth2[cardNum::fifthCard].size()) && (rankCounth1[cardNum::fourthCard].size() == matchedCards::two) && ((rankCounth1[cardNum::thirdCard].size() == rankCounth2[cardNum::thirdCard].size()) && (rankCounth1[cardNum::thirdCard].size() == matchedCards::two))){
				if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::fifthCard].rank){//compare the first pair
					return true;
				}
				if (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::fifthCard].rank){//the first pair is equal
					if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::thirdCard].rank){//compare the second pair
						return true;
					}
					if (h1.hand[cardNum::thirdCard].rank == h2.hand[cardNum::thirdCard].rank){//if second pair is also equal
						if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fourthCard].rank){//compare fifth card
							return true;
						}
					}
				}
				return false;
			}


			//1 1
			//2 2
			//2 2
			if ((rankCounth1[cardNum::fourthCard].size() == rankCounth2[cardNum::fourthCard].size()) && (rankCounth1[cardNum::fourthCard].size() == matchedCards::two) && ((rankCounth1[cardNum::thirdCard].size() == rankCounth2[cardNum::thirdCard].size()) && (rankCounth1[cardNum::thirdCard].size() == matchedCards::two))){
				if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::fourthCard].rank){//compare the first pair
					return true;
				}
				if (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::fourthCard].rank){//the first pair is equal
					if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::thirdCard].rank){//compare the second pair
						return true;
					}
					if (h1.hand[cardNum::thirdCard].rank == h2.hand[cardNum::thirdCard].rank){//if second pair is also equal
						if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fifthCard].rank){//compare fifth card
							return true;
						}
					}
				}
				return false;
			}
		}

		
		//One Pair
		//2 2
		//1 1
		//1 1
		//1 1
		if ((rankCounth1[cardNum::fifthCard].size() == rankCounth2[cardNum::fifthCard].size()) && (rankCounth1[cardNum::fifthCard].size() == matchedCards::two)){//compare h1[5th] and h2[5th];
			if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fifthCard].rank){//compare the first pair
				return true;
			}
			if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fifthCard].rank){//the first pair is equal
				if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::fourthCard].rank){//compare the second pair
					return true;
				}
				if (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::fourthCard].rank){//if second pair is also equal
					if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::thirdCard].rank){//compare fifth card
						return true;
					}
					if (h1.hand[cardNum::thirdCard].rank == h2.hand[cardNum::thirdCard].rank){//if second pair is also equal
						if (h1.hand[cardNum::secondCard].rank > h2.hand[cardNum::secondCard].rank){//compare fifth card
							return true;
						}
					}
				}
			}
			return false;
		}

		//2 1
		//1 2
		//1 1
		//1 1
		if ((rankCounth1[cardNum::fifthCard].size() == rankCounth2[cardNum::fourthCard].size()) && (rankCounth1[cardNum::fifthCard].size() == matchedCards::two)){
			if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fourthCard].rank){//compare the first pair
				return true;
			}
			if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fourthCard].rank){//the first pair is equal
				if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::fifthCard].rank){//compare the second pair
					return true;
				}
				if (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::fifthCard].rank){//if second pair is also equal
					if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::thirdCard].rank){//compare fifth card
						return true;
					}
					if (h1.hand[cardNum::thirdCard].rank == h2.hand[cardNum::thirdCard].rank){//if second pair is also equal
						if (h1.hand[cardNum::secondCard].rank > h2.hand[cardNum::secondCard].rank){//compare fifth card
							return true;
						}
					}
				}
			}
			return false;
		}

		//2 1
		//1 1
		//1 2
		//1 1
		if ((rankCounth1[cardNum::fifthCard].size() == rankCounth2[cardNum::thirdCard].size()) && (rankCounth1[cardNum::fifthCard].size() == matchedCards::two)){
			if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::thirdCard].rank){//compare the first pair
				return true;
			}
			if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::thirdCard].rank){//the first pair is equal
				if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::fifthCard].rank){//compare the second pair
					return true;
				}
				if (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::fifthCard].rank){//if second pair is also equal
					if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::fourthCard].rank){//compare fifth card
						return true;
					}
					if (h1.hand[cardNum::thirdCard].rank == h2.hand[cardNum::fourthCard].rank){//if second pair is also equal
						if (h1.hand[cardNum::secondCard].rank > h2.hand[cardNum::secondCard].rank){//compare fifth card
							return true;
						}
					}
				}
			}
			return false;
		}

		//2 1
		//1 1
		//1 1
		//1 2
		if ((rankCounth1[cardNum::fifthCard].size() == rankCounth2[cardNum::secondCard].size()) && (rankCounth1[cardNum::fifthCard].size() == matchedCards::two)){
			if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::secondCard].rank){//compare the first pair
				return true;
			}
			if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::secondCard].rank){//the first pair is equal
				if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::fifthCard].rank){//compare the second pair
					return true;
				}
				if (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::fifthCard].rank){//if second pair is also equal
					if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::fourthCard].rank){//compare fifth card
						return true;
					}
					if (h1.hand[cardNum::thirdCard].rank == h2.hand[cardNum::fourthCard].rank){//if second pair is also equal
						if (h1.hand[cardNum::secondCard].rank > h2.hand[cardNum::thirdCard].rank){//compare fifth card
							return true;
						}
					}
				}
			}
			return false;
		}
		

		//1 2
		//2 1
		//1 1
		//1 1
		if ((rankCounth1[cardNum::fourthCard].size() == rankCounth2[cardNum::fifthCard].size()) && (rankCounth1[cardNum::fourthCard].size() == matchedCards::two)){
			if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::fifthCard].rank){//compare the first pair
				return true;
			}
			if (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::fifthCard].rank){//the first pair is equal
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fourthCard].rank){//compare the second pair
					return true;
				}
				if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fourthCard].rank){//if second pair is also equal
					if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::thirdCard].rank){//compare fifth card
						return true;
					}
					if (h1.hand[cardNum::thirdCard].rank == h2.hand[cardNum::thirdCard].rank){//if second pair is also equal
						if (h1.hand[cardNum::secondCard].rank > h2.hand[cardNum::secondCard].rank){//compare fifth card
							return true;
						}
					}
				}
			}
			return false;
		}

		//1 1
		//2 2
		//1 1
		//1 1
		if ((rankCounth1[cardNum::fourthCard].size() == rankCounth2[cardNum::fourthCard].size()) && (rankCounth1[cardNum::fourthCard].size() == matchedCards::two)){
			if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::fourthCard].rank){//compare the first pair
				return true;
			}
			if (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::fourthCard].rank){//the first pair is equal
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fifthCard].rank){//compare the second pair
					return true;
				}
				if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fifthCard].rank){//if second pair is also equal
					if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::thirdCard].rank){//compare fifth card
						return true;
					}
					if (h1.hand[cardNum::thirdCard].rank == h2.hand[cardNum::thirdCard].rank){//if second pair is also equal
						if (h1.hand[cardNum::secondCard].rank > h2.hand[cardNum::secondCard].rank){//compare fifth card
							return true;
						}
					}
				}
			}
			return false;
		}


		//1 1
		//2 1
		//1 2
		//1 1
		if ((rankCounth1[cardNum::fourthCard].size() == rankCounth2[cardNum::thirdCard].size()) && (rankCounth1[cardNum::fourthCard].size() == matchedCards::two)){
			if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::thirdCard].rank){//compare the first pair
				return true;
			}
			if (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::thirdCard].rank){//the first pair is equal
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fifthCard].rank){//compare the second pair
					return true;
				}
				if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fifthCard].rank){//if second pair is also equal
					if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::fourthCard].rank){//compare fifth card
						return true;
					}
					if (h1.hand[cardNum::thirdCard].rank == h2.hand[cardNum::fourthCard].rank){//if second pair is also equal
						if (h1.hand[cardNum::secondCard].rank > h2.hand[cardNum::secondCard].rank){//compare fifth card
							return true;
						}
					}
				}
			}
			return false;
		}

		//1 1
		//2 1
		//1 1
		//1 2
		if ((rankCounth1[cardNum::fourthCard].size() == rankCounth2[cardNum::secondCard].size()) && (rankCounth1[cardNum::fourthCard].size() == matchedCards::two)){
			if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::secondCard].rank){//compare the first pair
				return true;
			}
			if (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::secondCard].rank){//the first pair is equal
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fifthCard].rank){//compare the second pair
					return true;
				}
				if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fifthCard].rank){//if second pair is also equal
					if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::fourthCard].rank){//compare fifth card
						return true;
					}
					if (h1.hand[cardNum::thirdCard].rank == h2.hand[cardNum::fourthCard].rank){//if second pair is also equal
						if (h1.hand[cardNum::secondCard].rank > h2.hand[cardNum::thirdCard].rank){//compare fifth card
							return true;
						}
					}
				}
			}
			return false;
		}

		//1 2
		//1 1
		//2 1
		//1 1
		if ((rankCounth1[cardNum::thirdCard].size() == rankCounth2[cardNum::fifthCard].size()) && (rankCounth1[cardNum::thirdCard].size() == matchedCards::two)){
			if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::fifthCard].rank){//compare the first pair
				return true;
			}
			if (h1.hand[cardNum::thirdCard].rank == h2.hand[cardNum::fifthCard].rank){//the first pair is equal
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fourthCard].rank){//compare the second pair
					return true;
				}
				if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fourthCard].rank){//if second pair is also equal
					if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::thirdCard].rank){//compare fifth card
						return true;
					}
					if (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::thirdCard].rank){//if second pair is also equal
						if (h1.hand[cardNum::secondCard].rank > h2.hand[cardNum::secondCard].rank){//compare fifth card
							return true;
						}
					}
				}
			}
			return false;
		}

		//1 1
		//1 2
		//2 1
		//1 1
		if ((rankCounth1[cardNum::thirdCard].size() == rankCounth2[cardNum::fourthCard].size()) && (rankCounth1[cardNum::thirdCard].size() == matchedCards::two)){
			if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::fourthCard].rank){//compare the first pair
				return true;
			}
			if (h1.hand[cardNum::thirdCard].rank == h2.hand[cardNum::fourthCard].rank){//the first pair is equal
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fifthCard].rank){//compare the second pair
					return true;
				}
				if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fifthCard].rank){//if second pair is also equal
					if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::thirdCard].rank){//compare fifth card
						return true;
					}
					if (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::thirdCard].rank){//if second pair is also equal
						if (h1.hand[cardNum::secondCard].rank > h2.hand[cardNum::secondCard].rank){//compare fifth card
							return true;
						}
					}
				}
			}
			return false;
		}

		//1 1
		//1 1
		//2 2
		//1 1
		if ((rankCounth1[cardNum::thirdCard].size() == rankCounth2[cardNum::thirdCard].size()) && (rankCounth1[cardNum::thirdCard].size() == matchedCards::two)){
			if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::thirdCard].rank){//compare the first pair
				return true;
			}
			if (h1.hand[cardNum::thirdCard].rank == h2.hand[cardNum::thirdCard].rank){//the first pair is equal
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fifthCard].rank){//compare the second pair
					return true;
				}
				if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fifthCard].rank){//if second pair is also equal
					if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::fourthCard].rank){//compare fifth card
						return true;
					}
					if (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::fourthCard].rank){//if second pair is also equal
						if (h1.hand[cardNum::secondCard].rank > h2.hand[cardNum::secondCard].rank){//compare fifth card
							return true;
						}
					}
				}
			}
			return false;
		}

		//1 1
		//1 1
		//2 1
		//1 2
		if ((rankCounth1[cardNum::thirdCard].size() == rankCounth2[cardNum::secondCard].size()) && (rankCounth1[cardNum::thirdCard].size() == matchedCards::two)){
			if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::secondCard].rank){//compare the first pair
				return true;
			}
			if (h1.hand[cardNum::thirdCard].rank == h2.hand[cardNum::secondCard].rank){//the first pair is equal
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fifthCard].rank){//compare the second pair
					return true;
				}
				if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fifthCard].rank){//if second pair is also equal
					if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::fourthCard].rank){//compare fifth card
						return true;
					}
					if (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::fourthCard].rank){//if second pair is also equal
						if (h1.hand[cardNum::secondCard].rank > h2.hand[cardNum::thirdCard].rank){//compare fifth card
							return true;
						}
					}
				}
			}
			return false;
		}

		//1 2
		//1 1
		//1 1
		//2 1
		if ((rankCounth1[cardNum::secondCard].size() == rankCounth2[cardNum::fifthCard].size()) && (rankCounth1[cardNum::secondCard].size() == matchedCards::two)){
			if (h1.hand[cardNum::secondCard].rank > h2.hand[cardNum::fifthCard].rank){//compare the first pair
				return true;
			}
			if (h1.hand[cardNum::secondCard].rank == h2.hand[cardNum::fifthCard].rank){//the first pair is equal
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fourthCard].rank){//compare the second pair
					return true;
				}
				if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fourthCard].rank){//if second pair is also equal
					if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::thirdCard].rank){//compare fifth card
						return true;
					}
					if (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::thirdCard].rank){//if second pair is also equal
						if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::secondCard].rank){//compare fifth card
							return true;
						}
					}
				}
			}
			return false;
		}

		//1 1
		//1 2
		//1 1
		//2 1
		if ((rankCounth1[cardNum::secondCard].size() == rankCounth2[cardNum::fourthCard].size()) && (rankCounth1[cardNum::secondCard].size() == matchedCards::two)){
			if (h1.hand[cardNum::secondCard].rank > h2.hand[cardNum::fourthCard].rank){//compare the first pair
				return true;
			}
			if (h1.hand[cardNum::secondCard].rank == h2.hand[cardNum::fourthCard].rank){//the first pair is equal
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fifthCard].rank){//compare the second pair
					return true;
				}
				if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fifthCard].rank){//if second pair is also equal
					if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::thirdCard].rank){//compare fifth card
						return true;
					}
					if (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::thirdCard].rank){//if second pair is also equal
						if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::secondCard].rank){//compare fifth card
							return true;
						}
					}
				}
			}
			return false;
		}

		//1 1
		//1 1
		//1 2
		//2 1
		if ((rankCounth1[cardNum::secondCard].size() == rankCounth2[cardNum::thirdCard].size()) && (rankCounth1[cardNum::secondCard].size() == matchedCards::two)){
			if (h1.hand[cardNum::secondCard].rank > h2.hand[cardNum::thirdCard].rank){//compare the first pair
				return true;
			}
			if (h1.hand[cardNum::secondCard].rank == h2.hand[cardNum::thirdCard].rank){//the first pair is equal
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fifthCard].rank){//compare the second pair
					return true;
				}
				if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fifthCard].rank){//if second pair is also equal
					if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::fourthCard].rank){//compare fifth card
						return true;
					}
					if (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::fourthCard].rank){//if second pair is also equal
						if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::secondCard].rank){//compare fifth card
							return true;
						}
					}
				}
			}
			
			return false;
		}

		//1 1
		//1 1
		//1 1
		//2 2
		if ((rankCounth1[cardNum::secondCard].size() == rankCounth2[cardNum::secondCard].size()) && (rankCounth1[cardNum::secondCard].size() == matchedCards::two)){
			if (h1.hand[cardNum::secondCard].rank > h2.hand[cardNum::secondCard].rank){//compare the first pair
				return true;
			}
			if (h1.hand[cardNum::secondCard].rank == h2.hand[cardNum::secondCard].rank){//the first pair is equal
				if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fifthCard].rank){//compare the second pair
					return true;
				}
				if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fifthCard].rank){//if second pair is also equal
					if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::fourthCard].rank){//compare fifth card
						return true;
					}
					if (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::fourthCard].rank){//if second pair is also equal
						if (h1.hand[cardNum::thirdCard].rank > h2.hand[cardNum::thirdCard].rank){//compare fifth card
							return true;
						}
					}
				}
			}
			return false;
		}//END One Pair

		//No Rank
		if (rankCounth1[cardNum::fifthCard].size() == matchedCards::one){
			if (h1.hand[cardNum::fifthCard].rank > h2.hand[cardNum::fifthCard].rank){
				return true;
			}
			if (h1.hand[cardNum::fifthCard].rank == h2.hand[cardNum::fifthCard].rank){
				if (h1.hand[cardNum::fourthCard].rank > h2.hand[cardNum::fourthCard].rank){
					return true;
				}
				if (h1.hand[cardNum::fourthCard].rank == h2.hand[cardNum::fourthCard].rank){
					if (h1.hand[(cardNum::thirdCard)].rank > h2.hand[(cardNum::thirdCard)].rank){
						return true;
					}
					if (h1.hand[(cardNum::thirdCard)].rank == h2.hand[(cardNum::thirdCard)].rank){
						if (h1.hand[(cardNum::secondCard)].rank > h2.hand[(cardNum::secondCard)].rank){
							return true;
						}
						if (h1.hand[(cardNum::secondCard)].rank == h2.hand[(cardNum::secondCard)].rank){
							if (h1.hand[cardNum::firstCard].rank > h2.hand[cardNum::firstCard].rank){
								return true;
							}
							else{
								return false;
							}
						}
					}
					
				}
			}
		}
	}
	return false; 
}
