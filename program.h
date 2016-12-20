//program.h 
#include "PlayingCard.h"
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#ifndef program_h
#define program_h

using namespace std;

int load(const char *cardFile, vector<Card> &deck);
int usageMessage(char * argv[], int argc);

/*Struct names returnVals to define return messages*/
struct returnVals{
	enum returnVal{ success = 0, usageMsg, errorIndecies, fewFileNum, manyFileNum, noCorrectGame, errorFileOpen, errorArguments, fewCards, errorHandNum, failure, decksEmpty};
};

struct argv_indecies{
	enum argv_index{program_name, firstArg, secondArg, min_argc = 4, max_argc = 11 };//firstArg and secondArg will be "-shuffle" and filename in either order
	
};

struct exceptions{
	enum exceptionName{instance_not_available =30, game_already_exists, unknown_game, no_game, already_playing, samePlayer};
};
#endif