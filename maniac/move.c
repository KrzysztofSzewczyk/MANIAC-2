
#include "move.h"

#include <stdlib.h>

struct move_t * new_move(void) {
	struct move_t * obj = malloc(sizeof(struct move_t));
	
	if(!obj)
		return NULL;
	
	obj->startrow = obj->endrow = obj->startcol = obj->endcol = obj->prom = 0;
	
	return obj;
}

struct move_t * new_move_coord(int x1, int y1, int x2, int y2) {
	struct move_t * obj = malloc(sizeof(struct move_t));
	
	if(!obj)
		return NULL;
	
	obj->startrow = x1;
	obj->endrow = y1;
	obj->startcol = x2;
	obj->endcol = y2;
	obj->prom = 0;
	
	return obj;
}

bool isvoid(struct move_t * this) {
	return (this->startrow + this->endrow + this->startcol + this->endcol) == 0;
}
