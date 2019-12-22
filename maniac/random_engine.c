
#include "move_checker.h"
#include "move.h"

#include <stdlib.h>

// WIP

struct move_t * random_engine(struct move_checker_t * c, int computer_color) {
	for (int i1 = 0; i1 < 6; i1++)
		for (int j1 = 0; j1 < 6; j1++)
			if (!move_checker_empty_square(c, i1, j1))
				if (move_checker_piece_owner(c->board[i1][j1]) == computer_color)
					for (int i2 = 0; i2 < 6; i2++)
						for (int j2 = 0; j2 < 6; j2++)
							for (int prom = 0; prom < 3; prom++)
								if (move_checker_semi_legal_move(c, i1, j1, i2, j2)) {
									struct move_checker_t * aftermove = new_move_checker_clone(c);
									move_checker_do_move(aftermove, 2 * prom + 3 + computer_color, i1, j1, i2, j2);
									
									if (move_checker_in_check(aftermove, computer_color))
										continue;
									
									struct move_t * m = new_move_coord(i1, j1, i2, j2);
									m->prom = 2 * prom + 3 + computer_color;
									return m;
								}
	return NULL;
}
