#include "Player.h"
#include <memory>
#include <vector>

#ifndef game_h
#define game_h

struct Player;
class Game{
public:
	static shared_ptr<Game*> instance();
	static void start_game(string const &);
	static void stop_game();
	void add_player(string &);
	shared_ptr <Player> find_player(string const &);
	virtual int before_turn(Player & player)=0;
	virtual int turn(Player & player)=0;
	virtual int after_turn(Player & player)=0;
	virtual int before_round()=0;
	virtual int after_round()=0;
	virtual int round()=0;
	int numPlayers();
	int resetChips(Player & player);
	void remove_player(string const &);
	static bool poker_rank_player(const shared_ptr<Player>& p1, const shared_ptr<Player>& p2);
protected:
	size_t dealer;
	unsigned int pot;
	unsigned int currentBet;
	unsigned int currentPot;
	bool betsNotCalled;
	bool skipRound;
	static shared_ptr<Game*> game;
	vector<shared_ptr<Player>> players;
	Deck mainDeck;
	void betChips(Player& player);
	int bettingRound();
	void leave();
	void join();
	void collectAnte();
	unsigned int counterFold; 
	
	

};//we're using shared pointers so we don't need a destructor
#endif