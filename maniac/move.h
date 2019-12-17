
#ifndef MOVE_H
#define MOVE_H

#include <stdbool.h>

struct move_t {
	int start_row, end_row;
	int start_col, end_col;
	int prom;
};

struct move_t * new_move(void);
struct move_t * new_move_coord(int x1, int y1, int x2, int y2);
bool is_void(struct move_t * this);

#endif
