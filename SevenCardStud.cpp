#include "stdafx.h"
#include "SevenCardStud.h"

using namespace std; 

//constructor
const int firstDeal = 2;
SevenCardStud::SevenCardStud(){
	dealer = 0;
	for (size_t i = 0; i <= Card::spades; ++i){
		for (size_t j = 0; j <= Card::ace; ++j){
			Card temp;
			temp.rank = Card::Rank(j);
			temp.suit = Card::Suit(i);
			mainDeck.add_card(temp);
		}
	}
}
int SevenCardStud::before_turn(Player & player) {
	if (roundNumber == 5){//JK don't harcode constants
		player.playerHandDown << mainDeck;
	}
	else{
		player.playerHandUp << mainDeck;
	}
	return returnVals::success;
}

int SevenCardStud::turn(Player & player){
	return returnVals::success;
}


int SevenCardStud::after_turn(Player & player){
	cout << player.playerName << ": " << endl;
	cout << player.playerHandUp << endl;
	return returnVals::success;
}
int SevenCardStud::before_round(){
	skipRound = false;
	currentBet = 0;
	currentPot = 0;
	counterFold = 0;
	roundNumber = 0;
	mainDeck.shuffleCards();
	collectAnte();
	int dealTo;
	for (size_t j = 0; j < firstDeal; ++j){
		for (size_t i = 0; i < players.size(); ++i){
			dealTo = (i + dealer + 1) % players.size();
			players[dealTo]->fold = false;
			players[dealTo]->playerHandDown << mainDeck;
		}
	}
	for (size_t i = 0; i < players.size(); ++i){
		before_turn(*players[i]);
	}
	for (size_t i = 0; i < players.size(); ++i) {
		after_turn(*players[i]);
	}
	roundNumber++;
	return returnVals::success;
}
int SevenCardStud::round(){
	if (skipRound){
		return returnVals::success;
	}
	currentBet = 0;
	currentPot = 0;
	int dealTo;
	if (roundNumber == 1){
		bettingRound();
	}
	else{
		for (size_t i = 0; i < players.size(); ++i){
			players[i]->lastBet = 0;
			dealTo = (i + dealer + 1) % players.size();
			if (players[dealTo]->fold == true){
				continue;
			}
			int result = before_turn(*players[dealTo]);
			if (result != returnVals::success){
				return returnVals::returnVal(result);
			}
		}
		for (size_t i = 0; i < players.size(); ++i){
			cout << players[i]->playerName << "'s visible cards: " << players[i]->playerHandUp << endl;
		}
		bettingRound();
	} 
	roundNumber++;
	return returnVals::success;
}

int SevenCardStud::after_round(){
	//LEFT: Figuring out a winner (Combination 7 choose 5 type thingy)
	//put players into tempPlayers if they haven't folded yet
	vector<shared_ptr<Player>> tempPlayers;
	for (size_t i = 0; i < players.size(); ++i){
		if (!players[i]->fold){
			tempPlayers.push_back(players[i]);
		}
		else{
			players[i]->handsLost += 1;
		}
		//Combine playerHandDown and playerHandUp
	}
	
	for (size_t i = 0; i < tempPlayers.size(); ++i){
		tempPlayers[i]->playerHand << tempPlayers[i]->playerHandUp;
		tempPlayers[i]->playerHand << tempPlayers[i]->playerHandDown;
	}
	if (tempPlayers.size() > 1){
		//Make all the combinations of possible 
		for (size_t i = 0; i < tempPlayers.size(); ++i){
			vector<Hand> possibleHands;
			possibleHands.push_back(getFiveCards(cardNums::one, cardNums::two, cardNums::three, cardNums::four, cardNums::five, *tempPlayers[i]));
			possibleHands.push_back(getFiveCards(cardNums::one, cardNums::two, cardNums::three, cardNums::four, cardNums::six, *tempPlayers[i]));
			possibleHands.push_back(getFiveCards(cardNums::one, cardNums::two, cardNums::three, cardNums::four, cardNums::seven, *tempPlayers[i]));
			possibleHands.push_back(getFiveCards(cardNums::one, cardNums::two, cardNums::three, cardNums::five, cardNums::six, *tempPlayers[i]));
			possibleHands.push_back(getFiveCards(cardNums::one, cardNums::two, cardNums::three, cardNums::five, cardNums::seven, *tempPlayers[i]));
			possibleHands.push_back(getFiveCards(cardNums::one, cardNums::two, cardNums::three, cardNums::six, cardNums::seven, *tempPlayers[i]));
			possibleHands.push_back(getFiveCards(cardNums::one, cardNums::two, cardNums::four, cardNums::five, cardNums::six, *tempPlayers[i]));
			possibleHands.push_back(getFiveCards(cardNums::one, cardNums::two, cardNums::four, cardNums::five, cardNums::seven, *tempPlayers[i]));
			possibleHands.push_back(getFiveCards(cardNums::one, cardNums::two, cardNums::four, cardNums::six, cardNums::seven, *tempPlayers[i]));
			possibleHands.push_back(getFiveCards(cardNums::one, cardNums::two, cardNums::five, cardNums::six, cardNums::seven, *tempPlayers[i]));
			possibleHands.push_back(getFiveCards(cardNums::one, cardNums::three, cardNums::four, cardNums::five, cardNums::six, *tempPlayers[i]));
			possibleHands.push_back(getFiveCards(cardNums::one, cardNums::three, cardNums::four, cardNums::five, cardNums::seven, *tempPlayers[i]));
			possibleHands.push_back(getFiveCards(cardNums::one, cardNums::three, cardNums::four, cardNums::six, cardNums::seven, *tempPlayers[i]));
			possibleHands.push_back(getFiveCards(cardNums::one, cardNums::three, cardNums::five, cardNums::six, cardNums::seven, *tempPlayers[i]));
			possibleHands.push_back(getFiveCards(cardNums::one, cardNums::four, cardNums::five, cardNums::six, cardNums::seven, *tempPlayers[i]));
			possibleHands.push_back(getFiveCards(cardNums::two, cardNums::three, cardNums::four, cardNums::five, cardNums::six, *tempPlayers[i]));
			possibleHands.push_back(getFiveCards(cardNums::two, cardNums::three, cardNums::four, cardNums::five, cardNums::seven, *tempPlayers[i]));
			possibleHands.push_back(getFiveCards(cardNums::two, cardNums::three, cardNums::four, cardNums::six, cardNums::seven, *tempPlayers[i]));
			possibleHands.push_back(getFiveCards(cardNums::two, cardNums::three, cardNums::five, cardNums::six, cardNums::seven, *tempPlayers[i]));
			possibleHands.push_back(getFiveCards(cardNums::two, cardNums::four, cardNums::five, cardNums::six, cardNums::seven, *tempPlayers[i]));
			possibleHands.push_back(getFiveCards(cardNums::three, cardNums::four, cardNums::five, cardNums::six, cardNums::seven, *tempPlayers[i]));
			sort(possibleHands.begin(), possibleHands.end(), poker_rank);
			for (int j = (cardNums::seven - 1); j >= 0; --j){
				tempPlayers[i]->playerHand.remove_card(j);
			}
			//The first hand in possibleHands will be the best
			tempPlayers[i]->playerHand = possibleHands[0];
		}
	}

	//now call poker rank on all iof the posibilities
	//now set that hand equal to the the playerHand
	if (tempPlayers.size() > 1){
		sort(tempPlayers.begin(), tempPlayers.end(), &Game::poker_rank_player);//sort players by their hand's poker rank //NOT WORKING CHECk
		for (size_t i = 0; i < tempPlayers.size(); ++i) {
			if (i == 0) {
				cout << tempPlayers[i]->playerName << " won this round." << endl;
				tempPlayers[i]->handsWon += 1;
				tempPlayers[i]->numChips += pot;
				pot = 0;
			}
			else {
				tempPlayers[i]->handsLost += 1;
			}
		}
		for (size_t i = 0; i < tempPlayers.size(); ++i) {
			cout << "Name: " << tempPlayers[i]->playerName << " Hands Won: " << tempPlayers[i]->handsWon << " Hands Lost: " << tempPlayers[i]->handsLost << "   Number of Chips: " << tempPlayers[i]->numChips << endl;
			cout << "Hand: " << tempPlayers[i]->playerHand << endl;
		}
		for (size_t i = 0; i < players.size(); ++i) {
			if (players[i]->fold){
				cout << "Name: " << players[i]->playerName << " Hands Won: " << players[i]->handsWon << " Hands Lost: " << players[i]->handsLost << "   Number of Chips: " << players[i]->numChips << endl;
			}
		}
	}
	else{
		for (size_t i = 0; i < players.size(); ++i){
			players[i]->handsLost += 1;
		}
		for (size_t i = 0; i < tempPlayers.size(); ++i){
			cout << tempPlayers[i]->playerName << " won this round, everyone else folded." << endl;
			tempPlayers[i]->handsWon += 1;
			tempPlayers[i]->handsLost -= 1;
			tempPlayers[i]->numChips += pot;
			pot = 0;
		}
		for (size_t i = 0; i < players.size(); ++i) {
			cout << "Name: " << players[i]->playerName << " Hands Won: " << players[i]->handsWon << " Hands Lost: " << players[i]->handsLost << "   Number of Chips: " << players[i]->numChips << endl;
		}
	}
	
	//Moving all cards from hands and discardDeck to mainDeck
	for (size_t i = 0; i < players.size(); ++i){
		for (int j = players[i]->playerHandDown.size() - 1; j >= 0; --j){
			mainDeck.add_card(players[i]->playerHandDown[j]);
			players[i]->playerHandDown.remove_card(j);
		}
		for (int j = players[i]->playerHandUp.size() - 1; j >= 0; --j){
			mainDeck.add_card(players[i]->playerHandUp[j]);
			players[i]->playerHandUp.remove_card(j);
		}
		for (int j = players[i]->playerHand.size() - 1; j >= 0; --j){
			players[i]->playerHand.remove_card(j);
		}
		
	}
	for (int i = discardDeck.size() - 1; i >= 0; --i){
		mainDeck.add_card(discardDeck.removeCard());
	}
	Game::leave();
	Game::join();
	if (dealer >= players.size() - 1) {
		dealer = 0;
	}
	else {
		++dealer;
	}

	//if a player is out of chips
	for (size_t i = 0; i < players.size(); ++i){
		resetChips(*players[i]);
	}

	return returnVals::success;
}

Hand SevenCardStud::getFiveCards(int one, int two, int three, int four, int five, Player &player){
	Hand tempHand;
	tempHand << player.playerHand[one];
	tempHand << player.playerHand[two];
	tempHand << player.playerHand[three];
	tempHand << player.playerHand[four];
	tempHand << player.playerHand[five];
	return tempHand;
}