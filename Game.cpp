#include "stdafx.h"
#include "Game.h"
#include "FiveCardDraw.h"
#include "SevenCardStud.h"
#include <vector>

using namespace std;
const size_t handSize = 5;
shared_ptr<Game*> Game::game=nullptr;
const unsigned int minBet = 1;
const unsigned int maxBet = 2;
void Game::start_game(string const & gameName){
	if (game){
		throw exceptions::game_already_exists;//
	}
	else if (gameName == "fivecarddraw"){
		FiveCardDraw * gameToPlay = new FiveCardDraw();
		game = make_shared<Game*>(gameToPlay);
		//dynamically allocate an instance of the FiveCardDraw class and store its address in the static pointer member variable
		//use make_shared()
	}
	else if (gameName == "sevencardstud"){
		SevenCardStud * gameToPlay = new SevenCardStud();
		game = make_shared<Game*>(gameToPlay);
		//dynamically allocate an instance of the FiveCardDraw class and store its address in the static pointer member variable
		//use make_shared()
	}
	else{
		throw exceptions::unknown_game;
	}
}
shared_ptr<Game*> Game::instance(){
	if (!game){
		throw exceptions::instance_not_available;//deal with this
	}
	else{
		return game;//return a copy of game //CHECKME
	}
}
void Game::stop_game(){
	if (!game){
		throw exceptions::no_game;
	}
	else{
		Game::game = nullptr;//set game to null
	}
}
void Game::add_player(string & name){
	bool notPlaying = true;
	for (unsigned i = 0; i < players.size(); ++i){
		if (name == players[i]->playerName){//name is already playing
			throw exceptions::already_playing;
			notPlaying = false;
		}
	}
	if (notPlaying){//dynamically allocate a Player with the name
		transform(name.begin(), name.end(), name.begin(), ::tolower);
		char * tempName = new char[name.length() + 1];
		for (size_t i = 0; i <= name.length(); ++i){
			tempName[i] = name[i];
		}
		Player * tempPlayer = new Player(tempName);
		shared_ptr<Player> newPlayer = make_shared<Player>(*tempPlayer);
		players.push_back(newPlayer);
		if (tempPlayer->numChips == 0){
			resetChips(*tempPlayer);
		}
	}
}
int Game::numPlayers(){
	return players.size();
}
void Game::remove_player(string const & name){
	for (unsigned i = 0; i < players.size(); ++i){
		if (name == players[i]->playerName){
			players.erase(players.begin()+i);
		}
	}
}
shared_ptr <Player> Game::find_player(string const & name){
	for (unsigned i = 0; i < players.size(); ++i){
		if (name == players[i]->playerName){
			return players[i];
		}
	}
	return nullptr;//return a singular pointer
}

void Game::leave() {
	string input;
	string word_1;
	bool no = false;
	cout << "Does anyone want to leave the game?" << endl;
	cout << "Ex: 'playername' Ex2: 'playername1' 'playername2' Ex3: 'no' (Will move on to the next round of poker)." << endl; 
	getline(cin, input);
	transform(input.begin(), input.end(), input.begin(), ::tolower);
	try{
		istringstream iss(input);
		while (iss >> word_1){

			if (word_1 == "no"){
				no = true;
				break;
			}
			shared_ptr<Player> leavingPlayer = find_player(word_1);
			if (leavingPlayer){
				//delete player from game
				string playerLeavingString = leavingPlayer->playerName + " " + to_string(leavingPlayer->handsWon) + " " + to_string(leavingPlayer->handsLost)+ " "+ to_string(leavingPlayer->numChips);
				string fileName = leavingPlayer->playerName + ".txt";
				ofstream ofs(fileName, std::ofstream::out);
				ofs << playerLeavingString;
				ofs.close();
				remove_player(word_1);
			}
			else{
				cout << "That Player does not exist. If no players wish to leave, enter 'no'. Otherwise, enter the names of the players that wish to leave seperated by a space" << endl;
			}

		}
		if (!no){
			leave();
		}
	}
	catch (exception e){
		cout << "If no players wish to leave, enter 'no'. Otherwise, enter the names of the players that wish to leave seperated by a space" << endl;
		leave();
	}
}

void Game::join() {
	string input;
	string word_1;
	const int maxGameSize = 9;
	bool join = false;
	cout << "Does anyone want to join the game?" << endl;
	cout << "Ex: 'playername' Ex2: 'playername1' 'playername2' Ex3: 'no' (Will move on to the next round of poker)." << endl;
	getline(cin, input);
	transform(input.begin(), input.end(), input.begin(), ::tolower);
	try{
		istringstream iss(input);
		while (iss >> word_1){
			if (word_1 == "no"){
				join = true;
				break;
			}
			if (players.size() >= maxGameSize && join == false){
				cout << "The table is full and players are no longer being added to the game. Say no to continue on to next round." << endl;
				break;
			}
			bool exists = false;
			for (unsigned int i = 0; i < players.size(); ++i){
				if (word_1 == players[i]->playerName){
					cout << word_1 << " exists and was not added" << endl;
					exists = true;
				}
			}
			if (!exists){
				add_player(word_1);
			}
		}
		if (!join){
			Game::join();
		}
	}
	catch (exception e){
		cout << "If no players wish to leave, enter 'no'. Otherwise, enter the names of the players that wish to leave seperated by a space" << endl;
		Game::join();
	}
}

void Game::collectAnte(){
	pot = 0;
	for (size_t i = 0; i < players.size(); ++i){
		players[i]->numChips--;
		++pot;
	}
	cout << "Current Pot Size: " << pot << endl;
}

int Game::bettingRound(){
	int dealTo;
	betsNotCalled = true;
	for (size_t i = 0; i < players.size(); ++i){
		dealTo = (i + dealer + 1) % players.size();
		if (players[dealTo]->fold == true || players[dealTo]->numChips == 0){
			continue;
		}
		cout << players[dealTo]->playerName << ": " << endl;
		cout << "Cards not visible: " << players[dealTo]->playerHandDown << endl;
		cout << "Visible cards: " << players[dealTo]->playerHandUp << endl;
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
				cout << "Cards not visible: " << players[dealTo]->playerHandDown << endl;
				cout << "Visible cards: " << players[dealTo]->playerHandUp << endl;
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
	return returnVals::success;
}

void Game::betChips(Player& player){
	if (currentBet == 0){
		cout << player.playerName << ", enter 'Check' or 'Bet'" << endl;
		cout << "You currently have " << player.numChips << " chips." << endl;
		string input1 = "";
		string input2 = "";
		getline(cin, input1);
		if (input1 == ""){
			betChips(player);
		}
		else{
			try{
				istringstream iss(input1);
				transform(input1.begin(), input1.end(), input1.begin(), ::tolower);
				if (input1 == "check"){
					cout << player.playerName << " has checked." << endl;
					player.matchedBet = true;
				}
				else if (input1 == "bet"){
					cout << player.playerName << ", enter 1 or 2." << endl;
					getline(cin, input2);
					istringstream buffer(input2);
					unsigned int bet;
					buffer >> bet;
					if (bet == minBet || bet == maxBet){
						if (bet > player.numChips){
							cout << "This is not a valid bet, you do not have enough chips." << endl;
							betChips(player);
						}
						else{
							player.numChips -= bet;
							pot += bet;
							currentBet += bet;
							player.lastBet = currentBet;
							for (size_t i = 0; i < players.size(); ++i){
								players[i]->matchedBet = false;
							}
							player.matchedBet = true;
							cout << "You bet " << bet << " chips." << endl;
							cout << "Current Pot Size: " << (pot + currentPot) << endl;
							cout << "Current High bet on table: " << currentBet << endl;
						}
					}
					else{
						cout << "This is not a valid bet, enter 0, 1, or 2." << endl;
						betChips(player);
					}
				}
				else{
					cout << player.playerName << ", that input was not valid. Enter 'Check' or 'Bet'" << endl;
					betChips(player);
				}
			}
			catch (exception e){
				cout << "Input either 0, 1, or 2 to indicate how many chips you would like to bet." << endl;
				betChips(player);
			}
		}
	}
	if(currentBet > player.lastBet){
		cout << player.playerName << ", enter 'Fold', 'Call', or 'Raise'." << endl;
		cout << "You currently have " << player.numChips << " chips." << endl;
		string input1 = "";
		string input2 = "";
		getline(cin, input1);
		if (input1 == ""){
			betChips(player);
		}
		else{
			try{
				istringstream iss(input1);
				transform(input1.begin(), input1.end(), input1.begin(), ::tolower);
				if (input1 == "fold"){
					cout << player.playerName << " has folded." << endl;
					//Fold member bool is true;
					player.fold = true;
					counterFold++; 
				}
				else if (input1 == "call"){
					player.numChips -= currentBet;
					currentPot += currentBet;
					player.lastBet = currentBet;
					player.matchedBet = true;
					cout << player.playerName << " has called " << currentBet << " chips." << endl;
					cout << "Current Pot Size: " << (pot+currentPot) << endl;
					cout << "Current High bet on table: " << currentBet << endl;
				}
				else if (input1 == "raise"){
					cout << player.playerName << ", raise 1 or 2 chips." << endl;
					getline(cin, input2);
					istringstream buffer(input2);
					unsigned int bet;
					buffer >> bet;
					if (bet == minBet || bet == maxBet){
						if ((bet + currentBet) > player.numChips){
							cout << "This is not a valid bet, you do not have enough chips." << endl;
							betChips(player);
						}
						else{
							player.numChips -= bet - currentBet;
							currentPot += bet + currentBet;
							currentBet += bet;
							player.lastBet = currentBet;
							for (size_t i = 0; i < players.size(); ++i){
								players[i]->matchedBet = false;
							}
							player.matchedBet = true;
							cout << "You placed " << currentBet << " chips in the pot." << endl;
							cout << "Current Pot Size: " << (pot + currentPot) << endl;
							cout << "Current High bet on table: " << currentBet << endl;
						}
					}
					else{
						cout << "This is not a valid bet, enter 1 or 2." << endl;
						betChips(player);
					}
				}
				else{
					cout << player.playerName << ", that input was not valid. Enter 'Fold', 'Call', or 'Raise'" << endl;
					betChips(player);
				}
			}
			catch (exception e){
				cout << "Input either 1 or 2 to indicate how many chips you would like to bet." << endl;
				betChips(player);
			}
		}
	}
}

int Game::resetChips(Player &player){
	if (player.numChips == 0){
		cout << player.playerName << ", you currently have " << player.numChips << " chips." << endl;
		cout << "Enter 'leave' to quit or 'reset' to get another 20 chips." << endl;
		string input1 = "";
		getline(cin, input1);
		if (input1 == ""){
			resetChips(player);
		}
		else{
			try{
				istringstream iss(input1);
				transform(input1.begin(), input1.end(), input1.begin(), ::tolower);
				if (input1 == "leave"){
					string playerLeavingString = player.playerName + " " + to_string(player.handsWon) + " " + to_string(player.handsLost) + " " + to_string(player.numChips);
					string fileName = player.playerName + ".txt";
					ofstream ofs(fileName, std::ofstream::out);
					ofs << playerLeavingString;
					ofs.close();
					remove_player(player.playerName);
				}
				if (input1 == "reset"){
					player.numChips = 20; //JK don't harcode constants
					cout << "Yor numChips have been reset to 20." << endl;
				}
				else{
					cout << "Your input is invalid. Enter 'leave' to quit or 'reset' to get another 20 chips." << endl;
					resetChips(player);
				}
			}
			catch (exception e){
				cout << "Your input is invalid. Enter 'leave' to quit or 'reset' to get another 20 chips." << endl;
				resetChips(player);
			}
		}
	}
	return returnVals::success;
}
bool Game::poker_rank_player(const shared_ptr<Player>& p1, const shared_ptr<Player>& p2){
	if (!p1){
		return false;
	}
	else if (!p2){
		return true;
	}
	else{
		return poker_rank(p1->playerHand, p2->playerHand);
	}
}
