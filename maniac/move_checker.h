
#ifndef MOVE_CHECKER_H
#define MOVE_CHECKER_H

#include "move.h"
#include "main.h"

#include <stdbool.h>

struct move_checker_t {
	int board[6][6];
	
	int kingrw, kingrb;
	
	bool white_short_castling, black_short_castling;
	bool white_long_castling, black_long_castling;
	
	int last_player_moved;
	
	struct move_t * last_move;
};

struct move_checker_t * new_move_checker(int board[6][6]);
struct move_checker_t * new_move_checker_clone(struct move_checker_t * clone);
bool move_checker_legal_square(struct move_checker_t * this, int col, int row);
bool move_checker_legal_move(struct move_checker_t * this, int p, int x1, int y1, int x2, int y2);
bool move_checker_legal_move_m(struct move_checker_t * this, int p, struct move_t * m);
bool move_checker_semi_legal_move(struct move_checker_t * this, int x1, int y1, int x2, int y2);
bool move_checker_legal_white_king_move(struct move_checker_t * this, int x1, int y1, int x2, int y2);
bool move_checker_legal_black_king_move(struct move_checker_t * this, int x1, int y1, int x2, int y2);
bool move_checker_legal_king_move(int x1, int y1, int x2, int y2);
bool move_checker_legal_rook_move(struct move_checker_t * this, int x1, int y1, int x2, int y2);
bool move_checker_legal_knight_move(struct move_checker_t * this, int x1, int y1, int x2, int y2);
bool move_checker_legal_bishop_move(struct move_checker_t * this, int x1, int y1, int x2, int y2);
bool move_checker_legal_queen_move(struct move_checker_t * this, int x1, int y1, int x2, int y2);
bool move_checker_legal_white_pawn_move(struct move_checker_t * this, int x1, int y1, int x2, int y2);
bool move_checker_legal_black_pawn_move(struct move_checker_t * this, int x1, int y1, int x2, int y2);
bool move_checker_empty_square(struct move_checker_t * this, int x, int y);
bool move_checker_taking_piece_of_same_color(struct move_checker_t * this, int x1, int y1, int x2, int y2);
void move_checker_change_castling_status(struct move_checker_t * this, int x, int y);
bool move_checker_legal_white_short_castling(struct move_checker_t * this);
bool move_checker_legal_white_long_castling(struct move_checker_t * this);
bool move_checker_legal_black_short_castling(struct move_checker_t * this);
bool move_checker_legal_black_long_castling(struct move_checker_t * this);
bool move_checker_pawn_promotion(struct move_checker_t * this, int x1, int y1, int y2);
void move_checker_set_last_player_moved(struct move_checker_t * this, int player);
int move_checker_get_last_player_moved(struct move_checker_t * this);
int move_checker_piece_owner(int piece);
int move_checker_piece_owner_cr(struct move_checker_t * this, int x, int y);
int move_checker_right_player(struct move_checker_t * this, int x, int y);
bool move_checker_in_check(struct move_checker_t * this, int player);

#endif
