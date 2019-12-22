
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

bool move_checker_legal_move(struct move_checker_t * this, int p, int c1, int r1, int c2, int r2) {
	return move_checker_semi_legal_move(this, c1, r1, c2, r2)
	&& move_checker_in_check_after_move(this, p, c1, r1, c2, r2);
}

bool move_checker_legal_move_m(struct move_checker_t * this, int p, struct move_t * m) {
	return move_checker_legal_move(this, p, m->start_col, m->start_row, m->end_col, m->end_row);
}

bool move_checker_semi_legal_move(struct move_checker_t * this, int c1, int r1, int c2, int r2) {
	if(!move_checker_legal_square(this, c2, r2))
		return false;
	
	if(c1 == c2 && r1 == r2)
		return false;
	
	if(move_checker_taking_piece_of_same_color(this, c1, r1, c2, r2))
		return false;
	
	switch(this->board[c1][r1]) {
	case UNDEFINED:
		return false;
	case BLACK_KING:
		return move_checker_legal_black_king_move(this, c1, r1, c2, r2);
	case WHITE_KING:
		return move_checker_legal_white_king_move(this, c1, r1, c2, r2);
	case WHITE_QUEEN: case BLACK_QUEEN:
		return move_checker_legal_queen_move(this, c1, r1, c2, r2);
	case WHITE_ROOK: case BLACK_ROOK:
		return move_checker_legal_rook_move(this, c1, r1, c2, r2);
	case WHITE_KNIGHT: case BLACK_KNIGHT:
		return move_checker_legal_knight_move(this, c1, r1, c2, r2);
	case BLACK_PAWN:
		return move_checker_legal_black_pawn_move(this, c1, r1, c2, r2);
	case WHITE_PAWN:
		return move_checker_legal_white_pawn_move(this, c1, r1, c2, r2);
	}
	
	return true;
}

bool move_checker_legal_white_king_move(struct move_checker_t * this, int c1, int r1, int c2, int r2) {
	if(c1 == this->kingrw && c2 == this->kingrw + 2 && r1 == 0 && r2 == 0)
		return move_checker_legal_white_short_castling(this);
	if(c1 == this->kingrw && c2 == this->kingrw - 2 && r1 == 0 && r2 == 0)
		return move_checker_legal_white_long_castling(this);
	return move_checker_legal_king_move(c1, r1, c2, r2);
}

bool move_checker_legal_black_king_move(struct move_checker_t * this, int c1, int r1, int c2, int r2) {
	if(c1 == this->kingrw && c2 == this->kingrw + 2 && r1 == 5 && r2 == 5)
		return move_checker_legal_black_short_castling(this);
	if(c1 == this->kingrw && c2 == this->kingrw - 2 && r1 == 5 && r2 == 5)
		return move_checker_legal_black_long_castling(this);
	return move_checker_legal_king_move(c1, r1, c2, r2);
}

bool move_checker_legal_king_move(int c1, int r1, int c2, int r2) {
	return ((c1 - c2) * (c1 - c2) <= 1 && (r1 - r2) * (r1 - r2) <= 1);
}

bool move_checker_legal_rook_move(struct move_checker_t * this, int c1, int r1, int c2, int r2) {
	if(c1 == c2) {
		for(int i = min(r1, r2) + 1; i < max(r1, r2); i++)
			if(!move_checker_empty_square(this, c1, i))
				return false;
		
		return true;
	} else if(r1 == r2) {
		for(int i = min(c1, c2) + 1; i < max(c1, c2); i++)
			if(!move_checker_empty_square(this, i, r1))
				return false;
		
		return true;
	} else
		return false;
}

bool move_checker_legal_knight_move(struct move_checker_t * this, int c1, int r1, int c2, int r2) {
	return abs((c1 - c2) * (r1 - r2)) == 2;
}

// Included just for completness, will be used by queen move check.
// Otherwise it's useless, as los alamos chess doesn't even have bishops.
bool move_checker_legal_bishop_move(struct move_checker_t * this, int c1, int r1, int c2, int r2) {
	int xdif = c1 - c2, ydif = r1 - r2;
	
	if (xdif == ydif || xdif == -ydif) {
		int xstep = (c1 < c2) ? 1 : -1,
		    ystep = (r1 < r2) ? 1 : -1,
			i = c1 + xstep,
			j = r1 + ystep;
		
		while (i != c2) {
			if (!move_checker_empty_square(this, i, j))
				return false;
			
			i += xstep;
			j += ystep;
		}
		
		return true;
	} else
		return false;
}

bool move_checker_legal_queen_move(struct move_checker_t * this, int c1, int r1, int c2, int r2) {
	return move_checker_legal_bishop_move(this, c1, r1, c2, r2)
	    || move_checker_legal_rook_move(this, c1, r1, c2, r2);
}

bool move_checker_legal_white_pawn_move(struct move_checker_t * this, int c1, int r1, int c2, int r2) {
	return (((this->board[c2][r2] != NONE) && r2 == r1 + 1 && ((c1 == c2 + 1) || (c1 == c2 - 1)))
	    || (!(this->board[c2][r2] != NONE) && (r2 == r1 + 1) && c1 == c2));
}

bool move_checker_legal_black_pawn_move(struct move_checker_t * this, int c1, int r1, int c2, int r2) {
	return (((this->board[c2][r2] != NONE) && r2 == r1 - 1 && ((c1 == c2 + 1) || (c1 == c2 - 1)))
	    || (!(this->board[c2][r2] != NONE) && (r2 == r1 - 1) && c1 == c2));
}

bool move_checker_empty_square(struct move_checker_t * this, int x, int y) {
	return this->board[x][y] == NONE;
}

bool move_checker_taking_piece_of_same_color(struct move_checker_t * this, int c1, int r1, int c2, int r2) {
	if(move_checker_empty_square(this, c2, r2))
		return false;
	
	// Black piece codes are divisible by two.
	return (this->board[c1][r1] & 1 == this->board[c2][r2] & 1);
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
	if(move_checker_in_check(this, 0))
		return false;
	
	for(int i = this->kingrw + 1; i < 5; i++)
		if(!move_checker_empty_square(this, i, 0) || move_checker_attacked(this, 1, i, 0))
			return false;
	
	return this->white_short_castling;
}

bool move_checker_legal_white_long_castling(struct move_checker_t * this) {
	if(move_checker_in_check(this, 0))
		return false;
	
	for(int i = this->kingrw - 1; i > 0; i--)
		if(!move_checker_empty_square(this, i, 0) || move_checker_attacked(this, 1, i, 0))
			return false;
	
	return this->white_long_castling;
}

bool move_checker_legal_black_short_castling(struct move_checker_t * this) {
	if(move_checker_in_check(this, 1))
		return false;
	
	for(int i = this->kingrb + 1; i < 5; i++)
		if(!move_checker_empty_square(this, i, 5) || move_checker_attacked(this, 0, i, 5))
			return false;
	
	return this->black_short_castling;
}

bool move_checker_legal_black_long_castling(struct move_checker_t * this) {
	if(move_checker_in_check(this, 1))
		return false;
	
	for(int i = this->kingrb - 1; i > 0; i--)
		if(!move_checker_empty_square(this, i, 5) || move_checker_attacked(this, 0, i, 5))
			return false;
	
	return this->black_long_castling;
}

bool move_checker_pawn_promotion(struct move_checker_t * this, int c1, int r1, int r2) {
	return (this->board[c1][r1] == WHITE_PAWN && r2 == 5) || (this->board[c1][r1] == BLACK_PAWN && r2 == 0);
}

void move_checker_set_last_player_moved(struct move_checker_t * this, int player) {
	this->last_player_moved = player;
}

int move_checker_get_last_player_moved(struct move_checker_t * this) {
	return this->last_player_moved;
}

int move_checker_piece_owner(int piece) {
	return ~(piece & 1);
}

int move_checker_piece_owner_cr(struct move_checker_t * this, int x, int y) {
	return move_checker_piece_owner(this->board[x][y]);
}

int move_checker_right_player(struct move_checker_t * this, int x, int y) {
	return move_checker_piece_owner_cr(this, x, y) != move_checker_get_last_player_moved(this);
}

bool move_checker_in_check(struct move_checker_t * this, int player) {
	int kingpiece = WHITE_KING + player;
	
	for (int x = 0; x < 6; x++)
		for (int y = 0; y < 6; y++)
			if (this->board[x][y] == kingpiece)
				for (int c2 = 0; c2 < 6; c2++)
					for (int r2 = 0; r2 < 6; r2++)
						if ((this->board[c2][r2] != NONE) || (this->board[c2][r2] != UNDEFINED))
							if (move_checker_semi_legal_move(this, c2, r2, x, y))
								return true;
	return false;
}

bool move_checker_attacked(struct move_checker_t * this, int player, int x, int y) {
	for (int c2 = 0; c2 < 6; c2++)
		for (int r2 = 0; r2 < 6; r2++)
			if ((!move_checker_empty_square(this, c2, r2)) || (this->board[c2][r2] != UNDEFINED))
				if (move_checker_piece_owner(this->board[c2][r2]) == player)
					if (move_checker_semi_legal_move(this, c2, r2, x, y))
						return true;
	
	return false;
}

bool move_checker_in_check_after_move(struct move_checker_t * this, int promote, int c1, int r1, int c2, int r2) {
	int player = move_checker_piece_owner_cr(this, c1, r1);
	struct move_checker_t * aftermove = new_move_checker_clone(this);
	move_checker_do_move(aftermove, promote, c1, r1, c2, r2);
	bool ret = move_checker_in_check(aftermove, player);
	free(aftermove);
	return ret;
}

void move_checker_do_move_m(struct move_checker_t * this, struct move_t * move) {
	move_checker_do_move(this, move->prom, move->start_col, move->start_row, move->end_col, move->end_row);
}

void move_checker_do_move(struct move_checker_t * this, int promote, int c1, int r1, int c2, int r2) {
	bool promotion = move_checker_pawn_promotion(this, c1, r1, r2);
	bool castled = !((this->board[c1][r1] > 2) || (abs(c1 - c2) < 2));
	
	if(this->last_move)
		free(this->last_move);
	
	this->last_move = new_move_coord(c1, r1, c2, r2);
	
	move_checker_set_last_player_moved(this, move_checker_piece_owner(this->board[c1][r1]));
	move_checker_change_castling_status(this, c1, r1);
	
	this->board[c2][r2] = this->board[c1][r1];
	this->board[c1][r1] = NONE;
	
	if (promotion)
		this->board[c2][r2] = move_checker_determine_promotion_piece(this, c2, r2, promote);
	
	if (castled)
		move_checker_handle_castling(this, c1, r1, c2, r2);
}

void move_checker_handle_castling(struct move_checker_t * this, int c1, int r1, int c2, int r2) {
	if(c1 > c2) {
		this->board[c2 + 1][r2] = this->board[0][r2];
		this->board[0][r2] = NONE;
	} else {
		this->board[c2 - 1][r2] = this->board[5][r2];
		this->board[5][r2] = 0;
	}
}

int move_checker_determine_promotion_piece(struct move_checker_t * this, int x, int y, int promote) {
	return promote + move_checker_piece_owner_cr(this, x, y);
}

void move_checker_disable_castling(struct move_checker_t * this) {
	this->white_short_castling = false;
	this->black_short_castling = false;
	
	this->white_long_castling = false;
	this->black_long_castling = false;
}

bool move_checker_mated(struct move_checker_t * this, int player) {
	return (this->last_player_moved != player
	   && move_checker_in_check(this, player)) ?
			(!move_checker_has_legal_move(this, player)) :
			false;
}

bool move_checker_stalemated(struct move_checker_t * this, int player) {
	return (this->last_player_moved != player
	   && !move_checker_in_check(this, player)) ?
			(!move_checker_has_legal_move(this, player)) :
			false;
}

bool move_checker_has_legal_move(struct move_checker_t * this, int player) {
	for(int x = 0; x < 6; x++) {
		for(int y = 0; y < 6; y++) {
			if(((!move_checker_empty_square(this, x, y))
					|| (this->board[x][y] != UNDEFINED))
				&& move_checker_piece_owner_cr(this, x, y) == player)
				for(int c2 = 0; c2 < 6; c2++)
					for(int r2 = 0; r2 < 6; r2++)
						if(move_checker_legal_move(this, 3, x, y, c2, r2))
							return true;
		}
	}
	
	return false;
}
