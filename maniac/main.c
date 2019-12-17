
#include "engines.h"
#include "main.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int main(void) {
	new_los_alamos(0);
}

struct los_alamos_t * new_los_alamos(int engine) {
	struct los_alamos_t * this = malloc(sizeof(struct los_alamos_t));
	
	if(!this)
		return NULL;
	
	this->setup = "rnqknr/pppppp///PPPPPP/RNQKNR";
	this->computer_color = 1;
	
	los_alamos_reset(this);
	
	switch(engine) {
		case 0:
			this->computer = materialistic_engine;
			break;
		
		case 1:
			this->computer = random_engine;
			break;
	}
	
	while(true) {
		for(int x = 0; x < 6; x++) {
			for(int y = 0; y < 6; y++)
				putchar(los_alamos_piece_to_char(this->board[x][y]));
			
			if(x != 5)
				putchar('/');
		}
		
		fflush(stdout);
		
		char l[6], len;
		len = scanf("%5s", l);
		
		if(len < 4)
			continue;
		
		int x2 = l[0]-'a', y2 = l[1]-'a', x1 = l[2]-'a', y1 = l[3]-'a';
		int promote = 3;
		
		if(len > 4) {
			promote = los_alamos_piece_to_char(toupper(l[4]));
			
			if(promote == 0)
				promote = 3;
		}
		
		// TODO: Make a move, check it, and respond with a computer move.
	}
	
	return this;
}

void los_alamos_reset(struct los_alamos_t * this) {
	if(this->move_number > 0)
		this->move_number--;
	
	for(int y = 0; y < 6; y++)
		for(int x = 0; x < 6; x++)
			this->board[x][y] = 0;
	
	los_alamos_load_fen(this);
	
	this->control = new_move_checker(this->board);
}

void los_alamos_load_fen(struct los_alamos_t * this) {
	int parseplace = 0, x = 0, y = 5;
	
	while(parseplace < strlen(this->setup)) {
		char c = this->setup[parseplace];
		
		if(c == '/') {
			x = 0;
			y--;
		} else if(c == '-') {
			this->board[x][y] = UNDEFINED;
			x++;
		} else if(isdigit(c) && c != '0') {
			int skip = c - '0';
			
			if(parseplace + 1 < strlen(this->setup)) {
				char next = this->setup[parseplace + 1];
				
				if(isdigit(next)) {
					parseplace++;
					skip = 10 * skip + next - '0';
				}
			}
			
			x += skip;
		} else if(x < 6 && y > -1) {
			this->board[x][y] = los_alamos_char_to_piece(c);
			x++;
		}
		
		parseplace++;
	}
}

// Here be dragons
int los_alamos_char_to_piece(int c) {
	const static int magic[] = {
		0x0AA6, 0xA824, 0xAAAA, 0xAAAA, 0xAAAA,
		0xAAAA, 0xAAAA, 0xAAAA, 0x1AA7, 0xA935
	};
	
	c -= 75;

	if(c > 39 || ((c >> 2) > 1 && (c >> 2) < 8))
		return 0xA;

    switch(c & 3) {
		case 0: return magic[c >> 2] >> 12;
		case 1: return (magic[c >> 2] & 0x0F00) >> 8;
		case 2: return (magic[c >> 2] & 0x00F0) >> 4;
		case 3: return (magic[c >> 2] & 0x000F);
	}
}

int los_alamos_piece_to_char(int c) {
	return (c>0&&c<11) ? "KkQqRrNnPp-"[c] : '.';
}
