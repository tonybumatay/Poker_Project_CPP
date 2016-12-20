#include "stdafx.h"
#include "Player.h"

using namespace std;

Player::Player(char *pName){//constructor
	string pTemp(pName);
	playerName = pTemp;
	handsWon = 0;
	handsLost = 0;
	fold = false;
	lastBet = 0;
	bool handsWonB = false;
	bool handsLostB = false;
	bool numChipsB = false;
	bool pNameCheck = false;
	bool nAN = false;
	numChips = 20;

	ifstream ifs;
	int counter=0;
	string playerFile =playerName + ".txt";
	ifs.open(playerFile);//open file input stream
	if (ifs.is_open()){
		string line_1, word_1;
		while (getline(ifs, line_1)){
			istringstream iss(line_1);
			while (iss >> word_1){//w
				int value = -1;
				transform(word_1.begin(), word_1.end(), word_1.begin(), ::tolower);
				transform(playerName.begin(), playerName.end(), playerName.begin(), ::tolower);
				if (counter > 0){
					istringstream buffer(word_1);
					if (buffer >> value){
					}
				}
				if ((counter== 0) && (word_1 == playerName)){//set the playerName
					playerName = word_1;
					pNameCheck = true;
				}
				if ((pNameCheck == false) && (counter > 0)){
					break;
				}
				if (counter == 1 && value >= 0){//handsWon Value must be greater than or equal to 0
					handsWon = value;
					handsWonB = true;
				}
				if (counter == 2 && value >= 0){//handsLost Value must be greater than or equal to 0
					handsLost = value;
					handsLostB = true;
				}
				if (counter == 3 && value >= 0){
					numChips = value;
					numChipsB = true;
				}
				++counter;
			}
			if ((pNameCheck == false) && (counter > 0)){
				break;
			}
		}
		
		if (!(handsWonB && handsLostB && numChipsB)){
			handsWon = 0;
			handsLost = 0;
			numChips = 20;
			 //the file did not contain all the required information
		}
		cout << "Player File Exists for " << playerName << ": handsWon: " << handsWon << " ; handsLost: " << handsLost << " numChips: "<< numChips<< endl ;
	}
	else{
		cout << "The player file does not exist for: " << playerName << endl;

		//doesn't open file
	}
}

ostream & operator<<(ostream & os, const Player &player){
	cout << player.playerName << " " << player.handsWon << " " << player.handsLost << endl;
	return os;
}
