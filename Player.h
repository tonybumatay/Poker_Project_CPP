#include "Hand.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#ifndef player_h
#define player_h

class Hand;
struct Player{
	friend ostream & operator<<(ostream &os, const Player &player);
	Player(char *pName);
	string playerName;
	Hand playerHand;
	Hand playerHandDown;
	Hand playerHandUp;
	unsigned int handsWon;
	unsigned int handsLost;
	unsigned int numChips;
	bool fold;
	bool matchedBet;
	unsigned int lastBet;
};

#endif