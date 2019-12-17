
#ifndef MOVE_CHECKER_H
#define MOVE_CHECKER_H

#include "main.h"

#include <stdbool.h>

struct move_checker_t {
	bool white_short_castling, black_short_castling;
	bool white_long_castling, black_long_castling;
};

struct move_checker_t * new_move_checker(int board[6][6]);

#endif
