
#include "move.h"

#include <stdlib.h>

struct move_t * new_move(void) {
	struct move_t * obj = malloc(sizeof(struct move_t));
	
	if(!obj)
		return NULL;
	
	obj->start_row = obj->end_row = obj->start_col = obj->end_col = obj->prom = 0;
	
	return obj;
}

struct move_t * new_move_coord(int x1, int y1, int x2, int y2) {
	struct move_t * obj = malloc(sizeof(struct move_t));
	
	if(!obj)
		return NULL;
	
	obj->start_row = x1;
	obj->end_row = y1;
	obj->start_col = x2;
	obj->end_col = y2;
	obj->prom = 0;
	
	return obj;
}

bool is_void(struct move_t * this) {
	return ~(this->start_row | this->end_row | this->start_col | this->end_col);
}
