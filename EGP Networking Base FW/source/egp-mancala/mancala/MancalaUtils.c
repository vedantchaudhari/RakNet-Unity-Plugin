/*
	Mancala game implementation
	Dan Buckstein
	Copyright 2017-2018

	Utilities source.
*/

#include "MancalaUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// draw game name in console
void MancalaPrintTitle()
{
	printf("\n");
	printf("\n MM    MM    AAAA    NN    NN    CCCCCC    AAAA    LL          AAAA   ");
	printf("\n MMM  MMM  AA    AA  NNNN  NN  CC        AA    AA  LL        AA    AA ");
	printf("\n MM MM MM  AAAAAAAA  NN NN NN  CC        AAAAAAAA  LL        AAAAAAAA ");
	printf("\n MM    MM  AA    AA  NN  NNNN  CC        AA    AA  LL        AA    AA ");
	printf("\n MM    MM  AA    AA  NN    NN    CCCCCC  AA    AA  LLLLLLLL  AA    AA ");
	printf("\n");
	printf("\n By Dan Buckstein (c) 2017-2018");
	printf("\n");
}

// clear and draw name
void MancalaPrintTitleClear()
{
	system("cls");
	MancalaPrintTitle();
}

// print board
void MancalaPrintBoard(
	int const *const *const countList, 
	char const *const *const playerNameList,
	int const playerIndex_top, 
	int const playerIndex_bottom)
{
	int value;
	int i;
	printf("\n");
	printf("\n %s\'s cups", playerNameList[playerIndex_top]);
	printf("\n  Score");
	printf("\n     ______________________________ ");

	printf("\n    |");
	for (i = 6; i >= 0; --i)
		printf((value = countList[playerIndex_top][i]) ? "%2d  " : "    ", value);
	printf("  |");

	printf("\n    |______________________________|");

	printf("\n    |  ");
	for (i = 0; i <= 6; ++i)
		printf((value = countList[playerIndex_bottom][i]) ? "  %2d" : "    ", value);
	printf("|");

	printf("\n    |______________________________|");
	printf("\n  #       1   2   3   4   5   6   SCORE");
	printf("\n %s\'s cups", playerNameList[playerIndex_bottom]);
	printf("\n");
}

// get player name from console
void MancalaGetPlayerName(
	char *const playerName_out)
{
	int const lenMax = mancala_name_len - 1;
	printf("Enter player name (%d chars max): ", lenMax);
	scanf("%s", playerName_out);
	playerName_out[lenMax] = 0;
}


// update total on each side
// returns 48 if both sides are still in play
// returns 0 if one side is out of stones
int MancalaUpdateTotal(
	int *const *const count)
{
	int *player0 = count[0], *player1 = count[1];
	int player0total = 0, player1total = 0, player0score, player1score;
	int i;

	// accumulate totals from first 6 slots on each side
	for (i = 0; i < 6; ++i)
	{
		player0total += *(player0++);
		player1total += *(player1++);
	}

	// grab scores for checksum
	player0score = *(player0++);
	player1score = *(player1++);

	// store total for each
	*player0 = player0total;
	*player1 = player1total;

	// return checksum; should always equal 48
	return (player0total + player1total + player0score + player1score);
}
