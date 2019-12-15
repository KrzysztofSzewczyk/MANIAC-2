
#ifndef MAIN_H
#define MAIN_H

#include "move_checker.h"
#include "engine.h"

struct los_alamos_t {
	int board[6][6];
	
	struct move_checker_t control;
	struct engine_t computer;
	
	int computer_color;
	int move_number;
	
	char * setup;
};

struct los_alamos_t * new_los_alamos(int engine);

#endif
