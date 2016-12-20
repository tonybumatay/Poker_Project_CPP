
#include "stdafx.h"
#include "Hand.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int usageMessage(char * argv[], int argc){
	if (argc < argv_indecies::min_argc){//too few. No command line args given
		cout << "Not enough arguements were received" << endl;
		return returnVals::fewFileNum;//return 4
	}
	if (argc > argv_indecies::max_argc){//too many. More than 11 command line args given
		cout << "Too many command line arguments were received." << endl;
		return returnVals::manyFileNum;//return 11
	}
	string gameName = argv[1];
	transform(gameName.begin(), gameName.end(), gameName.begin(), ::tolower);
	cout << "Usage: " << argv[argv_indecies::program_name] << " GameName (fiveCardDraw) and at least two player names" <<endl;
	//should take three arguments the name of the game and at least two player names
	
	if (gameName != "fivecarddraw" && gameName != "sevencardstud"){
		cout << "No correct game has been provided, please try again" << endl;
		return returnVals::noCorrectGame;
	}
	//Otherwise, main function should construct a deck obj using the input file name 


	return returnVals::usageMsg;//return 1
}