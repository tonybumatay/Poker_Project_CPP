#include "stdafx.h"
#include "FiveCardDraw.h"

using namespace std;

const size_t handSize = 5;

FiveCardDraw::FiveCardDraw(){//constructor
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





int FiveCardDraw::before_turn(Player & player) {
	if (player.fold == true){
		return returnVals::success;
	}
	cout << player.playerName << ": " << endl;
	cout << player.playerHand << endl;
	cout << player.playerName << ", which card(s) (if any) would you like to discard?" << endl;
	cout << "Usage: No input to discards zero cards" << endl;
	cout << "Put positions of card you would like to discard (0-4) seperated by a space" << endl;
	string input = "";
	string word_1;
	vector<int> cardsToDiscard;
	getline(cin, input);
	if (input == ""){
		return returnVals::success;
	}
	else{
		try{
			istringstream iss(input);
			while (iss >> word_1){
				bool replica = false;
				istringstream buffer(word_1);
				int discardIndex;
				buffer >> discardIndex;
				for (size_t i = 0; i < cardsToDiscard.size(); ++i){
					if (cardsToDiscard[i] == discardIndex){
						replica = true;
					}
				}
				if (replica){
					continue;
				}
				cardsToDiscard.push_back(discardIndex);
			}
			sort(cardsToDiscard.rbegin(), cardsToDiscard.rend());
			if (cardsToDiscard.size() > handSize){
				cout << "Please enter no more than 5 unique card positions (0-4)" << endl;
				before_turn(player);
			}
			for (size_t i = 0; i < cardsToDiscard.size(); ++i){
				if (cardsToDiscard[i] >= 0 && cardsToDiscard[i] < 5){
					discardDeck.add_card(player.playerHand[cardsToDiscard[i]]);
					player.playerHand.remove_card(cardsToDiscard[i]);
				}
				else{
					cout << cardsToDiscard[i] << " is not a valid input, not action was taken" << endl;
				}
			}
		}
		catch (exception e){
			cout << "Please only enter values that correspond to a position of a card (0-4) seperated by a space (No duplicates)" << endl;
			before_turn(player);
		}
	}
	return returnVals::success;
}

int FiveCardDraw::turn(Player & player){
	if (player.fold == true){
		return returnVals::success;
	}
	for (unsigned int i = player.playerHand.size(); i < handSize; ++i){
		if (mainDeck.size() > 0){
			player.playerHand << mainDeck;
		}
		else if (discardDeck.size() > 0){
			player.playerHand << discardDeck;
		}
		else {
			return returnVals::decksEmpty;
		}
	}
	return returnVals::success;
}

int FiveCardDraw::after_turn(Player & player){
	cout << player.playerName << ": " << endl;
	cout << player.playerHand << endl;
	return returnVals::success;
}

int FiveCardDraw::before_round(){
	skipRound = false;
	currentBet = 0;
	currentPot = 0;
	counterFold = 0;
	mainDeck.shuffleCards();
	collectAnte();
	int dealTo;
	for (size_t j = 0; j < handSize; ++j){
		for (size_t i = 0; i < players.size(); ++i){
			dealTo = (i + dealer + 1) % players.size();
			players[dealTo]->playerHand << mainDeck;
		}
	}
	for (size_t i = 0; i < players.size(); ++i){
		dealTo = (i + dealer + 1) % players.size();
		players[dealTo]->fold = false;
		players[dealTo]->lastBet = 0;
		if (players[dealTo]->numChips == 0){
			continue;
		}
		cout << players[dealTo]->playerName << ": " << endl;
		cout << players[dealTo]->playerHand << endl;
		Game::betChips(*players[dealTo]);
	}
	int counter = 0;
	for (size_t i = 0; i < players.size(); ++i){
		if (players[i]->fold == true || players[i]->matchedBet == true || players[i]->numChips == 0){
			counter++;
		}
	}
	if (counter == players.size()){
		betsNotCalled = false;
	}
	else{
		betsNotCalled = true;
	}
	if (counterFold == players.size() - 1){
		skipRound = true;
		return returnVals::success;
	}
	while (betsNotCalled){
		for (size_t i = 0; i < players.size(); ++i){
			dealTo = (i + dealer + 1) % players.size();
			if (players[dealTo]->fold == true || players[dealTo]->numChips == 0){
				continue;
			}
			if (!players[dealTo]->matchedBet){
				cout << players[dealTo]->playerName << ": " << endl;
				cout << players[dealTo]->playerHand << endl;
				Game::betChips(*players[dealTo]);
			}
			
		}
		counter = 0;
		for (size_t i = 0; i < players.size(); ++i){
			if (players[i]->fold == true || players[i]->matchedBet == true || players[i]->numChips == 0){
				counter++;
			}
		}
		if (counter == players.size()){
			betsNotCalled = false;
		}
		else{
			betsNotCalled = true;
		}
		if (counterFold == players.size() - 1){
			skipRound = true;
			return returnVals::success;
		}
	}
	for (size_t i = 0; i < players.size(); ++i){
		dealTo = (i + dealer + 1) % players.size();
		before_turn(*players[dealTo]);
	}
	pot += currentPot;
	return returnVals::success;
}

int FiveCardDraw::round(){
	if (skipRound){
		return returnVals::success;
	}
	currentBet = 0;
	currentPot = 0;
	int dealTo;
	for (size_t i = 0; i < players.size(); ++i){
		players[i]->lastBet = 0;
		dealTo = (i + dealer + 1) % players.size();
		int result = turn(*players[dealTo]);
		if (result != returnVals::success){
			return returnVals::returnVal(result);
		}
	}
	betsNotCalled = true;
	for (size_t i = 0; i < players.size(); ++i){
		dealTo = (i + dealer + 1) % players.size();
		if (players[dealTo]->fold == true || players[dealTo]->numChips == 0){
			continue;
		}
		cout << players[dealTo]->playerName << ": " << endl;
		cout << players[dealTo]->playerHand << endl;
		Game::betChips(*players[dealTo]);
	}
	int counter = 0;
	for (size_t i = 0; i < players.size(); ++i){
		if (players[i]->fold == true || players[i]->matchedBet == true || players[i]->numChips == 0){
			counter++;
		}
	}
	if (counter == players.size()){
		betsNotCalled = false;
	}
	else{
		betsNotCalled = true;
	}
	if (counterFold == players.size() - 1){
		skipRound = true;
		return returnVals::success;
	}
	while (betsNotCalled){
		for (size_t i = 0; i < players.size(); ++i){
			dealTo = (i + dealer + 1) % players.size();
			if (players[dealTo]->fold == true || players[dealTo]->numChips == 0){
				continue;
			}
			if (!players[dealTo]->matchedBet){
				cout << players[dealTo]->playerName << ": " << endl;
				cout << players[dealTo]->playerHand << endl;
				Game::betChips(*players[dealTo]);
			}
		}
		counter = 0;
		for (size_t i = 0; i < players.size(); ++i){
			if (players[i]->fold == true || players[i]->matchedBet == true || players[i]->numChips == 0){
				counter++;
			}
		}
		if (counter == players.size()){
			betsNotCalled = false;
		}
		else{
			betsNotCalled = true;
		}
		if (counterFold == players.size() - 1){
			skipRound = true;
			return returnVals::success;
		}
	}
	pot += currentPot;
	currentPot = 0;
	for (size_t i = 0; i < players.size(); ++i){
		dealTo = (i + dealer + 1) % players.size();
		after_turn(*players[dealTo]);
	}
	return returnVals::success;
}

int FiveCardDraw::after_round(){
	vector<shared_ptr<Player>> tempPlayers;
	for (size_t i = 0; i < players.size(); ++i){
		if (!players[i]->fold){
			tempPlayers.push_back(players[i]);
		}
		else{
			players[i]->handsLost += 1;
		}
	}
	if (tempPlayers.size() > 1){
		sort(tempPlayers.begin(), tempPlayers.end(), &FiveCardDraw::poker_rank_player);//sort players by their hand's poker rank //NOT WORKING CHECk
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
		for (int j = handSize - 1; j >= 0; --j){
			mainDeck.add_card(players[i]->playerHand[j]);
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

