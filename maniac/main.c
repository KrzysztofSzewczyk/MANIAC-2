
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
	
	// Note: It's ugly as heck, I should rewrite it once I get
	//       more stuff finished.
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
		
		if(move_checker_legal_move(this->control, promote, x2, y2, x1, y1) && move_checker_right_player(this->control, x2, y2)) {
			move_checker_do_move(this->control, promote, x2, y2, x1, y1);
			
			this->move_number++;
			
			struct move_t * computer_move = this->computer(this->control, this->computer_color);
			
			if(!computer_move) {
				printf("The engine resigned.");
				break;
			}
			
			move_checker_do_move_m(this->control, computer_move);
			
			if(move_checker_mated(this->control, 1 - this->computer_color))
				printf("0-1");
			else if(move_checker_stalemated(this->control, 1 - this->computer_color))
				printf("1/2-1/2");
			else if(move_checker_mated(this->control, this->computer_color))
				printf("1-0");
			else if(move_checker_stalemated(this->control, this->computer_color))
				printf("1/2-1/2");
			else
				continue;
			
			break;
		}
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
		case 0: return (magic[c >> 2] >> 12) + 1;
		case 1: return ((magic[c >> 2] & 0x0F00) >> 8) + 1;
		case 2: return ((magic[c >> 2] & 0x00F0) >> 4) + 1;
		case 3: return (magic[c >> 2] & 0x000F) + 1;
	}
}

int los_alamos_piece_to_char(int c) {
	return (c>0&&c<12) ? "KkQqRrNnPp-"[c-1] : '.';
}
