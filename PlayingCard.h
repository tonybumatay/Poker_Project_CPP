
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#ifndef playingCard_h
#define playingCard_h


struct Card{
	enum Suit { clubs=0, diamonds, hearts, spades };
	enum Rank { two = 0, three, four, five, six, seven, eight, nine, ten, jack, queen, king, ace };
	Suit suit;
	Rank rank;
	bool operator<(const Card& c) const;
};

#endif

