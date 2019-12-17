
#include "board_state.h"

struct board_state_t * new_board_state(struct move_checker_t * copy) {
	struct board_state_t * this = malloc(sizeof(struct board_state_t));
	
	if(!this)
		return NULL;
	
	this->white_short_castling = copy->white_short_castling;
	this->white_long_castling = copy->white_long_castling;
	this->black_short_castling = copy->black_short_castling;
	this->black_long_castling = copy->black_long_castling;
	
	this->last_player_moved = copy->last_player_moved;
	
	this->last_move = new_move_coord(
		copy->last_move.start_col, copy->last_move.start_row,
		copy->last_move.end_col, copy->last_move.end_row
	);
	
	this->last_move->prom = copy->last_move->prom;
	
	for(int i = 0; i < 6; i++)
		for(int j = 0; j < 6; j++)
			this->board[i][j] = copy->board[i][j];
	
	return this;
}
