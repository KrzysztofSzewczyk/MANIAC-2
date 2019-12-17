
#ifndef MAIN_H
#define MAIN_H

#include "move_checker.h"
#include "pieces.h"

struct los_alamos_t {
	int board[6][6];
	
	struct move_checker_t * control;
	struct move_t * (*computer)(struct move_checker_t *, int);
	
	int computer_color;
	int move_number;
	
	char * setup;
};

struct los_alamos_t * new_los_alamos(int engine);
void los_alamos_reset(struct los_alamos_t * this);
void los_alamos_load_fen(struct los_alamos_t * this);

int los_alamos_char_to_piece(int c);
int los_alamos_piece_to_char(int c);

#endif
