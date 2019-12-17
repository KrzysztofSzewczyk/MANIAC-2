
#ifndef ENGINES_H
#define ENGINES_H

#include "move_checker.h"
#include "move.h"

extern struct move_t * materialistic_engine(struct move_checker_t *, int);
extern struct move_t * random_engine(struct move_checker_t *, int);

#endif
