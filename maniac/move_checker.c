
#include "pieces.h"
#include "move_checker.h"

#include <stdlib.h>

struct move_checker_t * new_move_checker(int board[6][6]) {
	struct move_checker_t * this = malloc(sizeof(struct move_checker_t));
	
	if(!this)
		return NULL;
	
	for(int i = 0; i < 6; i++)
		for(int j = 0; j < 6; j++)
			this->board[i][j] = board[i][j];
	
	this->white_short_castling = true;
	this->black_short_castling = true;
	
	this->white_long_castling = true;
	this->black_long_castling = true;
	
	this->last_player_moved = 1;
	this->last_move = new_move();
	
	this->kingrw = 4;
	this->kingrb = 4;
	
	for(int i = 0; i < 6; i++)
		if(board[i][0] == WHITE_KING)
			this->kingrw = i;
	
	for(int i = 0; i < 6; i++)
		if(board[i][5] == BLACK_KING)
			this->kingrb = i;
	
	return this;
}
