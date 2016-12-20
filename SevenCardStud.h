#include "Game.h"

#ifndef sevencardstud_h
#define sevencardstud_h

class SevenCardStud : public  Game{
public:
	SevenCardStud();
	int before_turn(Player & player);
	int turn(Player & player);
	int after_turn(Player & player);
	int before_round();
	int after_round();
	int round();
	static bool poker_rank_player(const shared_ptr<Player>& p1, const shared_ptr<Player>& p2);
	Hand getFiveCards(int one, int two, int three, int four, int five, Player &player);
protected : 
	Deck discardDeck;
	unsigned int roundNumber;
};

struct cardNums{
	enum cardNum{ one = 0, two, three, four, five, six, seven };
};
#endif