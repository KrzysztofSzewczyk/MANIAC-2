
#ifndef MOVE_CHECKER_H
#define MOVE_CHECKER_H

#include "move.h"
#include "main.h"

#include <stdbool.h>

struct move_checker_t {
	int board[6][6];
	
	int kingrw, kingrb;
	
	bool white_short_castling, black_short_castling;
	bool white_long_castling, black_long_castling;
	
	int last_player_moved;
	
	struct move_t * last_move;
};

struct move_checker_t * new_move_checker(int board[6][6]);

#endif
