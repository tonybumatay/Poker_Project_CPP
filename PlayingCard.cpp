//PlayingCard.cpp: playing card source file to define a struct to represent a playing card of type "card" that declares 
#include "stdafx.h"
#include "program.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


using namespace std;
//we have a file parsing function that takes in 1)a STL vector of card structs (of type vector<Card> &) and 2) a C-style string (of type char *)
//The function uses the string as a parameter for the name of the file, opens the file, and then reads one card definition string at a time from the file until it is empty
//each time a valid card def. string is read (ignore invalid strings) the function pushes back a card struct instance with the rank and suit encoded in the string, into the vector
//that was passed as the first parameter. If the file cannot be opened of there are other issues, print out a helpful error message indicating the problem and return a 
//non-zero integer value; otherwise return 0 to indecate success

bool Card::operator<(const Card& c) const{
	if (rank < c.rank){
		return true;
	}
	else if (rank == c.rank){
		if (suit < c.suit){
			return true;
		}
	}
	return false;
}

