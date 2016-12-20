Lab number: 4
Graded by: Joseph Kwon
Score: 68 / 100 
Comments:

-1 h:\cse332\sp15_lab4\sp15_lab4\hand.cpp(102): warning C4018: '<' : signed/unsigned mismatch
-1 h:\cse332\sp15_lab4\sp15_lab4\deck.cpp(40): warning C4244: 'argument' : conversion from 'time_t' to 'unsigned int', possible loss of data
-2 document tests in readme
-2 minimal comments (say what unobvious parts of your code is doing in FiveCardDraw.cpp, Game.cpp, and SevenCardStud.cpp)
-0 join == false in line 146 in Game.cpp can be just !join
-3 don't harcode constants (search //JK to find where, only marked 3)
-3 fails obvious case where there's only two players with the same name
-3 accepts same player name and game doesn't end
-1 didn't fix accepting same player from lab 3
-0 usage message implies only game is FiveCardDraw
-1 cosmetic issue: not displaying * for other people's facedowns
-3 betting something then having to call subtracts too much (subtracts entire call cost, not call - what they already bet)
-3 raising doesn't subtract at all sometimes adds?
-1 folding with only 2 players causes the folded player to have lose 2 hands, not 1 (may happen with more people, only tested for 2)
-3 doesn't save correctly if all but one player leaves (the player that didn't leave doesn't get saved although game ends) thus fails to load as well
-5 infinite loop when player leaves and comes back, first player bets, then rest folds. Constantly prints out visible hands 

--------------------------------------------


========================================================================

    CONSOLE APPLICATION : Lab3 Project Overview

========================================================================

ANTHONY (TONY) BUMATAY, HANNAH MEHRLE, IAN BERNSTEIN LAB 3

Files: 

SevenCardStud.cpp

	SevenCardStud allows players to interactively play Seven Card Stud with any amount from 2-9 players. This class derives from the Game class

	and implements all required pure virtual methods. If errors arise within the program, we attempt to catch them and reprompt the user rather

	than having it crash (this occurs in the when using cin). The betting is implemented in the same manner as FiveCardDraw, with 5 rounds of betting,

	one after each round of dealing. The rest of the implementation is done as specified by the lab 4 document. 

FiveCardDraw.cpp

	FiveCardDraw allows players to interactively play Five Card Draw with any amount from 2-9 players. This class derives from the Game class

	and implements all required pure virtual methods. If errors arise within the program, we attempt to catch them and reprompt the user rather

	than having it crash (this occurs in the when using cin). 

Game.cpp

	The Game class is an abstract base class that we can use to implement any type of poker game. Currently we are using it to implement Five Card Draw 

	and Seven Card Stud. This class runs the behind the scenes work and storage for the FiveCardDraw and SevenCardStud classes. We use shared_ptrs to 

	implement the game.

Player.cpp

	A Player class that gives a player a name, Hand of cards, number of hands Won value, number of hands Lost value, and number of chips. 

	The constructor takes in a C-style string as the Player Name, initializes the numWins and numLosses to 0 and numChips to 20. Each player also has

	a fold boolean to track whether or not he/she has folded the current hand. The constructor then tries to open 

	a file with the same name as the player. If the file cannot be opened the program continues to run. 

Hand.cpp

	Hand is implemented as directed by the previous lab. We also added two insertion operators. The first takes two references to different hands

	and inserts the first into the second. The second operator takes a reference to a card and a reference to a hand and inserts the card into the

	hand. 

Deck.cpp

	Deck is implemented as directed in the previous lab. 

program.cpp

	The usage message is created in this file. The Usage message prints out a helpful message

	that tells the user what arguments to provide to the command line. For the following cases

	a unique error message was thrown, the usage message is printes, and the program stops. 

	The two supported values for GameName are FiveCardDraw and SevenCardStud (not case sensitive).

	//too few. Less than 4 total arguments (Correct numArgs: Lab3.exe, GameName, 2-9 space-separated players)

	//too many. More than 11 total arguments

Lab4.cpp

	Our main method first checks to see if the usage message detected an error in with the command line 

	arguments as detailed above in program.cpp. If an error is detected the usage message prints and 

	a unique integer is returned. After, we create an instance of the game provided in the argv as the second argument (currently

	this program supports five card draw and seven card stud as acceptable game types). Next it repeatedly calls before_round, round, after_round while more

	than two players exist in the game. The game will stop when less than two players remain. 

Errors and Warnings we ran into:

	We ran into an error with trying to use strcopy on a char * C style string. To avoid that we ended up using a for loop to solve the issue.

	The big errors we ran into were pertaining to the use of the shared_ptr and abstract base classes. The solution we found is to 

	have shared_ptr<Game*>. While this requires dereferencing twice, it allowed our program to build and prevented instances 

	of the abstract base Game class from being created which preventing the program from compiling. 

	We also ran into signed/unsigned mismatched errors which were qiuckly resolved. 



	We ran into an array of other errors and warnings, but we were able to resolve the rest of them successfully.  



	Joke of the Day: Two fish are in a tank. One turns to the other and says "Who's driving?".
