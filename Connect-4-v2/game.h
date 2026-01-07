#ifndef CONNECT4_GAME_H
#define CONNECT4_GAME_H

#include "board.h"
#include "input.h"

/**
 * Initialize and run a Player vs Player game
 * Handles turn switching, move validation, and win detection
 */
void game_init_pvp(void);

/**
 * Initialize and run Player vs Computer - Easy difficulty
 * AI makes random valid moves
 */
void game_init_easy(void);

/**
 * Initialize and run Player vs Computer - Medium difficulty
 * AI blocks threats and makes strategic moves
 */
void game_init_medium(void);

/**
 * Initialize and run Player vs Computer - Hard difficulty
 * AI uses minimax algorithm for optimal play
 */
void game_init_hard(void);

/**
 * Check if the last move resulted in a win
 * Returns 1 if winner found (4 in a row), 0 otherwise
 */
int check_winner(GameMove last_move, Cell player);

/**
 * Check for vertical 4-in-a-row from last move
 */
int check_vertical(GameMove last_move, Cell player);

/**
 * Check for horizontal 4-in-a-row from last move
 */
int check_horizontal(GameMove last_move, Cell player);

/**
 * Check for diagonal 4-in-a-row from last move
 */
int check_diagonal(GameMove last_move, Cell player);

#endif /* CONNECT4_GAME_H */