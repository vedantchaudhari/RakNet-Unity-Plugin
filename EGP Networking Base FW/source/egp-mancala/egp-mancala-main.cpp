/*
	Mancala game implementation
	Dan Buckstein
	Copyright 2017-2018

	Mancala rules: 
	1. Board setup

	Player 2 (index 1)
	 Score	     6     5     4     3     2     1
		      <---  <---  <---  <---  <---  <---  <---
		 ________________________________________________
		| /  \  \__/  \__/  \__/  \__/  \__/  \__/  /  \ |
		| \__/  \__/  \__/  \__/  \__/  \__/  \__/  \__/ |
		|________________________________________________|
	Player 1 (index 0)
		Cup	ID	  1     2     3     4     5     6     SCORE ("mancala")
		Direction of play: 
	        --->  --->  --->  --->  --->  --->  --->  

	Each regular cup (ID 1-6) starts with 4 stones each. 
	Mancala cups (score) start empty.

	2. Play
	Taking turns, a player removes all of the stones from one of their own 
	cups with stones in it, and redistributes the stones around the board, one 
	at a time in the counter-clockwise direction, starting with the next cup. 
	Stones are placed in the player's own mancala (score cup) but not in the 
	opponent's mancala. If the last stone lands in the player's mancala, the 
	player goes again. If the last stone lands in an empty cup on the player's 
	own side, the player may choose to steal all of the stones directly across 
	from the landed by moving them into the landed cup.

	3. End condition
	The game ends when a player has no stones on their side of the board. When 
	this happens, the opponent takes all of the stones on their side and moves 
	them into their mancala. The winner is the player with more stones in 
	their mancala.
*/


#include "mancala/MancalaUtils.h"

#include <stdio.h>
#include <stdlib.h>


// take a turn
// returns resulting player and cup indices as pointers
// returns pointer to cup that can be stolen from as pointer (null if not)
// return value is 1 if the player goes again (landed in their mancala)
int MancalaTurn(int *const *const count, int playerIndex, int cupIndex, 
	int *const playerIndex_result, int *const cupIndex_result, int **const steal_ptr)
{
	const int activePlayer = playerIndex;

	// get stone count and reset the selected cup
	int stones = count[playerIndex][cupIndex];
	count[playerIndex][cupIndex] = 0;

	// while count is not zero, distribute around the board
	while (stones)
	{
		// switch sides if we are on a mancala
		if (cupIndex == 6)
		{
			playerIndex = (playerIndex + 1) % 2;
			cupIndex = -1;
		}

		// go to the next cup
		// if cup is not a mancala, increment its count
		if (++cupIndex < 6)
		{
			++count[playerIndex][cupIndex];
			--stones;
		}
		
		// if the cup is a mancala, increment count if we are on our side
		// if we are not on our side, the loop resets and switches sides
		else if (activePlayer == playerIndex)
		{
			++count[playerIndex][cupIndex];
			--stones;
		}
	}

	// store outputs
	*playerIndex_result = playerIndex;
	*cupIndex_result = cupIndex;

	// if landed in our cup with count of 1 (was previously empty), get 
	//	pointer to the cup across from it
	if (playerIndex == activePlayer && cupIndex < 6 && count[playerIndex][cupIndex] == 1)
		*steal_ptr = count[(activePlayer + 1) % 2] + 5 - cupIndex;
	else
		*steal_ptr = 0;

	// if we landed in our mancala, go again
	return (cupIndex == 6 ? 1 : 0);
}


// Mancala game: straight-forward, two human players
// returns index of winner, -1 if tie
// saves final scores in optional pointer passed as argument (scores_out)
int MancalaSimple(const char *const *const playerName, int *const scores_out)
{
	// core game state: 
	// stone counters for each player
	// playerCount[player index][cup index]
	// cup indices 0-5 used for play, 6 for score and 7 for total on-side
	int playerBuffer[16] = {
		4, 4, 4, 4, 4, 4, 0, 24,	// player 0: starts at element 0
		4, 4, 4, 4, 4, 4, 0, 24,	// player 1: starts at element 8
	};
	int *playerCount[] = { (playerBuffer + 0), (playerBuffer + 8) };
	int activePlayer = 0, otherPlayer = 1, cupIndex = 0, winner = -1;
	int turnResult, playerIndexResult[1], cupIndexResult[1], *stealResult[1];


	// ****GAME LOOP**** 
	// while both sides have stones, players alternate turns
	while (playerCount[0][7] && playerCount[1][7] && playerCount[0][6] < 25 && playerCount[1][6] < 25)
	{
		MancalaPrintTitleClear();
		MancalaPrintBoard(playerCount, playerName, otherPlayer, activePlayer);

		printf("\n%s\'s turn! ", playerName[activePlayer]);
		do
		{
			printf("Pick a cup with stones (1-6): ");
			cupIndex = getchar() - 48;
			while (getchar() != '\n');
			turnResult = cupIndex > 0 && cupIndex <= 6 ? playerCount[activePlayer][--cupIndex] : 0;
		} while (turnResult == 0);

		// take a turn
		turnResult = MancalaTurn(playerCount, activePlayer, cupIndex, playerIndexResult, cupIndexResult, stealResult);

		// handle steal if landed in empty cup with stones across from it
		if (*stealResult && **stealResult)
		{
			printf("Landed in empty cup, steal\? 0 = no, other = yes: ");
			cupIndex = getchar() - 48;
			while (getchar() != '\n');
			if (cupIndex)
			{
				playerCount[activePlayer][*cupIndexResult] += **stealResult;
				**stealResult = 0;
			}
		}

		// switch players
		if (!turnResult)
		{
			otherPlayer = activePlayer;
			activePlayer = (activePlayer + 1) % 2;
		}

		// update counts
		cupIndex = MancalaUpdateTotal(playerCount);

		if (cupIndex != 48)
			printf("\n failed checksum");
	}

	// final display
	MancalaPrintTitleClear();
	MancalaPrintBoard(playerCount, playerName, otherPlayer, activePlayer);

	// end of game, put any leftovers in that player's mancala
	playerCount[0][6] += playerCount[0][7];
	playerCount[1][6] += playerCount[1][7];

	// pass scores
	if (scores_out)
	{
		scores_out[0] = playerCount[0][6];
		scores_out[1] = playerCount[1][6];
	}

	// game over, determine winner
	winner = (playerCount[0][6] > playerCount[1][6] ? 0 : playerCount[0][6] < playerCount[1][6] ? 1 : -1);
	return winner;
}


// entry function
int main(int const argc, char const *const *const argv)
{
	char playerNameBuffer[2][mancala_name_len] = { "Jim", "Bob" };
	char *playerName[] = { playerNameBuffer[0], playerNameBuffer[1] };
	int scores[2] = { 0 };
	int winner;


	// startup: get player names
	MancalaPrintTitleClear();
	printf("\n Player 1: ");
	MancalaGetPlayerName(playerName[0]);
	printf("\n Player 2: ");
	MancalaGetPlayerName(playerName[1]);
	while (getchar() != '\n');

	do
	{
		// play a local multiplayer game of Mancala
		winner = MancalaSimple(playerName, scores);
		printf(winner >= 0 ? "\n %s wins!" : "\n Tie game!", playerName[winner]);
		printf("     %s %d - %d %s", playerName[0], scores[0], scores[1], playerName[1]);
		printf("\n Play again\? 0 = no, other = yes: ");
		winner = getchar() - 48;
		while (getchar() != '\n');
	} while (winner);

	// exit
	MancalaPrintTitleClear();
	printf("\n\n");
	system("pause");
}
