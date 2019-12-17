
#ifndef BOARD_STATE_H
#define BOARD_STATE_H

#include "move.h"

struct board_state_t {
	int board[6][6];
	
	int last_player_moved;
	bool white_short_castling, black_short_castling;
	bool white_long_castling, black_long_castling;
	
	struct move_t last_move;
};

struct board_state_t * new_board_state(struct move_checker_t * copy);

#endif
