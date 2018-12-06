/*
	Mancala game implementation
	Dan Buckstein
	Copyright 2017-2018

	Utilities include file.
*/

#ifndef _MANCALAUTILS_H_
#define _MANCALAUTILS_H_


#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif	// !_CRT_SECURE_NO_WARNINGS


#ifdef __cplusplus
extern "C"
{
#endif	// __cplusplus

	
//-----------------------------------------------------------------------------

	enum
	{
		mancala_name_len = 16,
	};


//-----------------------------------------------------------------------------

	// draw game name in console
	void MancalaPrintTitle();

	// clear and draw name
	void MancalaPrintTitleClear();

	// print board
	//	param countList: arrays of totals (the game state to display)
	//	param playerNameList: array of player names
	//	param playerIndex_top: top (inactive) player index
	//	param playerIndex_bottom: bottom (active) player index
	void MancalaPrintBoard(
		int const *const *const countList, 
		char const *const *const playerNameList, 
		int const playerIndex_top, 
		int const playerIndex_bottom
	);

	// get player name from console
	//	param playerName_out: character array to capture user input
	void MancalaGetPlayerName(
		char *const playerName_out
	);

	// update total on each side
	//	param count: array of pointers to players' totals
	//	returns 48 if both sides are still in play
	//	returns 0 if one side is out of stones
	int MancalaUpdateTotal(
		int *const *const count
	);

	
//-----------------------------------------------------------------------------


#ifdef __cplusplus
}
#endif	// __cplusplus


#endif	// !_MANCALAUTILS_H_