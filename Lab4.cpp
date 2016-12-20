#include "stdafx.h"
#include "FiveCardDraw.h"
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;
const int sevenCardStudRounds = 5; 
int main(int argc, char * argv[]){
	int usageResult = usageMessage(argv, argc);//checks to make sure arguments are legitimate
	if ((usageResult == returnVals::fewFileNum)){
		return returnVals::fewFileNum;//return unique error int
	}
	if ((usageResult == returnVals::manyFileNum)){
		return returnVals::manyFileNum;//return unique error int
	}
	if ((usageResult == returnVals::noCorrectGame)){
		return returnVals::noCorrectGame;
	}
	string gameName = argv[1];
	transform(gameName.begin(), gameName.end(), gameName.begin(), ::tolower);
	const string lowerCaseGame = gameName;
	Game::start_game(lowerCaseGame);
	shared_ptr<Game*> game = Game::instance();
	for (int i = 2; i < argc; ++i){
		(*game)->add_player(string(argv[i]));
	}
	if (gameName == "fivecarddraw") {
		try{
			while ((*game)->numPlayers() > 1){
				(*game)->before_round();
				(*game)->round();
				(*game)->after_round();
			}
			(*game)->stop_game();//end the game if there are less than two players
			return returnVals::success;//return 0
		}
		catch (int e){//catch the exceltion from the load function call in the deck constructor
			(*game)->stop_game();//end the game if and exception occurs
			if (e == returnVals::errorFileOpen){
				cout << "An exception occurred. The file did not open. Exception Number: " << e << endl;
				return returnVals::errorFileOpen;
			}
			else if (e == returnVals::errorHandNum){
				cout << "An exception occurred. You tried to access an index that does not exist. Exception Number: " << e << endl;
				return returnVals::errorHandNum;
			}
			else {
				cout << "An exception occurred. Exception Number: " << e << endl;
				return returnVals::failure;
			}
		}
	}
	else if (gameName == "sevencardstud") {
		try{
			while ((*game)->numPlayers() > 1){
				(*game)->before_round();
				for (size_t i = 0; i < sevenCardStudRounds; ++i) {
					(*game)->round();
				}
				(*game)->after_round();
			}
			(*game)->stop_game();//end the game if there are less than two players
			return returnVals::success;//return 
		}
		catch (int e) {
			(*game)->stop_game();//end the game if and exception occurs
			if (e == returnVals::errorFileOpen){
				cout << "An exception occurred. The file did not open. Exception Number: " << e << endl;
				return returnVals::errorFileOpen;
			}
			else if (e == returnVals::errorHandNum){
				cout << "An exception occurred. You tried to access an index that does not exist. Exception Number: " << e << endl;
				return returnVals::errorHandNum;
			}
			else {
				cout << "An exception occurred. Exception Number: " << e << endl;
				return returnVals::failure;
			}
		}
	}
}
//END MAIN
