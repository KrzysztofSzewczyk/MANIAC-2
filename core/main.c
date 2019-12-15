
#include "main.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void) {
	new_los_alamos(0);
}

struct los_alamos_t * new_los_alamos(int engine) {
	struct los_alamos_t * this = malloc(sizeof(struct los_alamos_t));
	
	if(!this)
		return NULL;
	
	this->setup = "rnqknr/pppppp///PPPPPP/RNQKNR";
	this->computercolor = 1;
	this->computer = new_engine();
	
	los_alamos_reset();
	
	switch(engine) {
		case 0:
			engine_set(this->computer, materialistic_engine);
			break;
		
		case 1:
			engine_set(this->computer, random_engine);
			break;
	}
	
	while(true) {
		for(int x = 0; x < 6; x++) {
			for(int y = 0; y < 6; y++)
				putchar(los_alamos_piece_to_char(this->board[y][x]));
			
			if(x != 5)
				putchar('/');
		}
		
		fflush(stdout);
		
		char l[6];
		scanf("%5s", l);
		
		if(strlen(l) < 4)
			continue;
		
		int x2 = l[0]-'a', y2 = l[1]-'a', x1 = l[2]-'a', y1 = l[3]-'a';
		int promote = 3;
		
		if(strlen(l) > 4) {
			promote = los_alamos_piece_to_char(toupper(l[4]));
			
			if(promote == 0)
				promote = 3;
		}
		
		
	}
	
	return this;
}
