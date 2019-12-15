
#ifndef MOVE_H
#define MOVE_H

#include <stdbool.h>

struct move_t {
	int startrow;
	int endrow;
	int startcol;
	int endcol;
	int prom;
};

struct move_t * new_move(void);
struct move_t * new_move_coord(int x1, int y1, int x2, int y2);
bool isvoid(struct move_t * this);

#endif
