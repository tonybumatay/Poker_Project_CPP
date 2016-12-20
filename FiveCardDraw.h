#include "Game.h"

#ifndef fiveCardDraw_h
#define fiveCardDraw_h

class FiveCardDraw : public  Game{ 
public:
	FiveCardDraw();
	int before_turn(Player & player);
	int turn(Player & player);
	int after_turn(Player & player);
	int before_round();
	int after_round();
	int round(); 
protected:
	Deck discardDeck;
};

#endif