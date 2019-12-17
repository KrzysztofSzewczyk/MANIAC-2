
#ifndef MAIN_H
#define MAIN_H

#include "move_checker.h"

struct los_alamos_t {
	int board[6][6];
	
	struct move_checker_t control;
	struct move_t (*computer)(struct los_alamos_t, struct move_checker_t, int);
	
	int computer_color;
	int move_number;
	
	char * setup;
};

#define BLACK_KING 0
#define WHITE_KING 1
#define BLACK_QUEEN 2
#define WHITE_QUEEN 3
#define BLACK_ROOK 4
#define WHITE_ROOK 5
#define BLACK_KNIGHT 6
#define WHITE_KNIGHT 7
#define BLACK_PAWN 8
#define WHITE_PAWN 9
#define UNDEFINED 10

struct los_alamos_t * new_los_alamos(int engine);
void los_alamos_reset(struct los_alamos_t * this);
void los_alamos_load_fen(struct los_alamos_t * this);

int los_alamos_char_to_piece(int c);
int los_alamos_piece_to_char(int c);

#endif
