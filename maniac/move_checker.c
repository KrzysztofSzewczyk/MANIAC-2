
#include "pieces.h"
#include "move_checker.h"
#include "board_state.h"
#include "move.h"

#include <stdlib.h>

#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) > (b)) ? (b) : (a))
#define abs(a)   (((a) < 0) ? (-(a)) : (a))

struct move_checker_t * new_move_checker(int board[6][6]) {
	struct move_checker_t * this = malloc(sizeof(struct move_checker_t));
	
	if(!this)
		return NULL;
	
	for(int i = 0; i < 6; i++)
		for(int j = 0; j < 6; j++)
			this->board[i][j] = board[i][j];
	
	this->white_short_castling = true;
	this->black_short_castling = true;
	
	this->white_long_castling = true;
	this->black_long_castling = true;
	
	this->last_player_moved = 1;
	this->last_move = new_move();
	
	this->kingrw = 4;
	this->kingrb = 4;
	
	for(int i = 0; i < 6; i++)
		if(board[i][0] == WHITE_KING)
			this->kingrw = i;
	
	for(int i = 0; i < 6; i++)
		if(board[i][5] == BLACK_KING)
			this->kingrb = i;
	
	return this;
}

struct move_checker_t * new_move_checker_clone(struct move_checker_t * clone) {
	struct move_checker_t * this = malloc(sizeof(struct move_checker_t));
	
	if(!this)
		return NULL;
	
	for(int i = 0; i < 6; i++)
		for(int j = 0; j < 6; j++)
			this->board[i][j] = clone->board[i][j];
	
	this->white_short_castling = clone->white_short_castling;
	this->black_short_castling = clone->black_short_castling;
	
	this->white_long_castling = clone->white_long_castling;
	this->black_long_castling = clone->black_long_castling;
	
	this->last_player_moved = clone->last_player_moved;
	this->last_move = new_move_coord(
		clone->last_move->start_col, clone->last_move->start_row,
		clone->last_move->end_col, clone->last_move->end_row
	);
	
	return this;
}

bool move_checker_legal_square(struct move_checker_t * this, int col, int row) {
	return col > 0 && col < 6 && row > 0 && row < 6 && this->board[col][row] != UNDEFINED;
}

bool move_checker_legal_move(struct move_checker_t * this, int p, int x1, int y1, int x2, int y2) {
	return move_checker_semi_legal_move(this, x1, y1, x2, y2)
	&& move_checker_in_check_after_move(this, p, x1, y1, x2, y2);
}

bool move_checker_legal_move_m(struct move_checker_t * this, int p, struct move_t * m) {
	return move_checker_legal_move(this, p, m->start_col, m->start_row, m->end_col, m->end_row);
}

bool move_checker_semi_legal_move(struct move_checker_t * this, int x1, int y1, int x2, int y2) {
	if(!move_checker_legal_square(this, x2, y2))
		return false;
	
	if(x1 == x2 && y1 == y2)
		return false;
	
	if(move_checker_taking_piece_of_same_color(this, x1, y1, x2, y2))
		return false;
	
	switch(this->board[x1][y1]) {
	case UNDEFINED:
		return false;
	case BLACK_KING:
		return move_checker_legal_black_king_move(this, x1, y1, x2, y2);
	case WHITE_KING:
		return move_checker_legal_white_king_move(this, x1, y1, x2, y2);
	case WHITE_QUEEN: case BLACK_QUEEN:
		return move_checker_legal_queen_move(this, x1, y1, x2, y2);
	case WHITE_ROOK: case BLACK_ROOK:
		return move_checker_legal_rook_move(this, x1, y1, x2, y2);
	case WHITE_KNIGHT: case BLACK_KNIGHT:
		return move_checker_legal_knight_move(this, x1, y1, x2, y2);
	case BLACK_PAWN:
		return move_checker_legal_black_pawn_move(this, x1, y1, x2, y2);
	case WHITE_PAWN:
		return move_checker_legal_white_pawn_move(this, x1, y1, x2, y2);
	}
	
	return true;
}

bool move_checker_legal_white_king_move(struct move_checker_t * this, int x1, int y1, int x2, int y2) {
	if(x1 == this->kingrw && x2 == this->kingrw + 2 && y1 == 0 && y2 == 0)
		return move_checker_legal_white_short_castling(this);
	if(x1 == this->kingrw && x2 == this->kingrw - 2 && y1 == 0 && y2 == 0)
		return move_checker_legal_white_long_castling(this);
	return move_checker_legal_king_move(x1, y1, x2, y2);
}

bool move_checker_legal_black_king_move(struct move_checker_t * this, int x1, int y1, int x2, int y2) {
	if(x1 == this->kingrw && x2 == this->kingrw + 2 && y1 == 5 && y2 == 5)
		return move_checker_legal_black_short_castling(this);
	if(x1 == this->kingrw && x2 == this->kingrw - 2 && y1 == 5 && y2 == 5)
		return move_checker_legal_black_long_castling(this);
	return move_checker_legal_king_move(x1, y1, x2, y2);
}

bool move_checker_legal_king_move(int x1, int y1, int x2, int y2) {
	return ((x1 - x2) * (x1 - x2) <= 1 && (y1 - y2) * (y1 - y2) <= 1);
}

bool move_checker_legal_rook_move(struct move_checker_t * this, int x1, int y1, int x2, int y2) {
	if(x1 == x2) {
		for(int i = min(y1, y2) + 1; i < max(y1, y2); i++)
			if(!move_checker_empty_square(this, x1, i))
				return false;
		
		return true;
	} else if(y1 == y2) {
		for(int i = min(x1, x2) + 1; i < max(x1, x2); i++)
			if(!move_checker_empty_square(this, i, y1))
				return false;
		
		return true;
	} else
		return false;
}

bool move_checker_legal_knight_move(struct move_checker_t * this, int x1, int y1, int x2, int y2) {
	return abs((x1 - x2) * (y1 - y2)) == 2;
}

// Included just for completness, will be used by queen move check.
// Otherwise it's useless, as los alamos chess doesn't even have bishops.
bool move_checker_legal_bishop_move(struct move_checker_t * this, int x1, int y1, int x2, int y2) {
	int xdif = x1 - x2, ydif = y1 - y2;
	
	if (xdif == ydif || xdif == -ydif) {
		int xstep = (x1 < x2) ? 1 : -1,
		    ystep = (y1 < y2) ? 1 : -1,
			i = x1 + xstep,
			j = y1 + ystep;
		
		while (i != x2) {
			if (!move_checker_empty_square(this, i, j))
				return false;
			
			i += xstep;
			j += ystep;
		}
		
		return true;
	} else
		return false;
}

bool move_checker_legal_queen_move(struct move_checker_t * this, int x1, int y1, int x2, int y2) {
	return move_checker_legal_bishop_move(this, x1, y1, x2, y2)
	    || move_checker_legal_rook_move(this, x1, y1, x2, y2);
}

bool move_checker_legal_white_pawn_move(struct move_checker_t * this, int x1, int y1, int x2, int y2) {
	return (((this->board[x2][y2] != NONE) && y2 == y1 + 1 && ((x1 == x2 + 1) || (x1 == x2 - 1)))
	    || (!(this->board[x2][y2] != NONE) && (y2 == y1 + 1) && x1 == x2));
}

bool move_checker_legal_black_pawn_move(struct move_checker_t * this, int x1, int y1, int x2, int y2) {
	return (((this->board[x2][y2] != NONE) && y2 == y1 - 1 && ((x1 == x2 + 1) || (x1 == x2 - 1)))
	    || (!(this->board[x2][y2] != NONE) && (y2 == y1 - 1) && x1 == x2));
}

bool move_checker_empty_square(struct move_checker_t * this, int x, int y) {
	return this->board[x][y] == NONE;
}

bool move_checker_taking_piece_of_same_color(struct move_checker_t * this, int x1, int y1, int x2, int y2) {
	if(move_checker_empty_square(this, x2, y2))
		return false;
	
	// Black piece codes are divisible by two.
	return (this->board[x1][y1] & 1 == this->board[x2][y2] & 1);
}

void move_checker_change_castling_status(struct move_checker_t * this, int x, int y) {
	if(x == 0 && y == 0)
		this->white_long_castling = false;
	
	if(x == 5 && y == 0)
		this->white_short_castling = false;
	
	if(x == 0 && y == 5)
		this->black_long_castling = false;
	
	if(x == 5 && y == 0)
		this->black_short_castling = false;
	
	if(this->board[x][y] == WHITE_KING)
		this->white_long_castling = this->white_short_castling = false;
	
	if(this->board[x][y] == BLACK_KING)
		this->black_long_castling = this->black_short_castling = false;
}

bool move_checker_legal_white_short_castling(struct move_checker_t * this) {
	if(move_checker_in_check(0))
		return false;
	
	for(int i = this->kingrw + 1; i < 5; i++)
		if(!move_checker_empty_square(this, i, 0) || move_checker_attacked(this, 1, i, 0))
			return false;
	
	return this->white_short_castling;
}

bool move_checker_legal_white_long_castling(struct move_checker_t * this) {
	if(move_checker_in_check(0))
		return false;
	
	for(int i = this->kingrw - 1; i > 0; i--)
		if(!move_checker_empty_square(this, i, 0) || move_checker_attacked(this, 1, i, 0))
			return false;
	
	return this->white_long_castling;
}
