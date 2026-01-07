#ifndef CONNECT4_AI_H
#define CONNECT4_AI_H

#include "input.h"
#include "board.h"

/**
 * Find a winning move for the specified player
 * Simulates each possible move and checks if it wins
 * Returns: Move with row/col, or {-1,-1} if no winning move
 */
GameMove find_winning_move(Cell player);

/**
 * AI Easy difficulty - plays random valid moves
 * No strategic thinking, purely random column selection
 */
GameMove ai_easy_move(void);

/**
 * AI Medium difficulty
 * Strategy: Win when possible, block threats, make strategic moves
 * Includes 30% randomness for unpredictability
 */
GameMove ai_medium_move(void);

/**
 * AI Hard difficulty - uses Minimax algorithm
 * Looks ahead several moves to find optimal play
 * Most challenging opponent
 */
GameMove ai_hard_move(void);

#endif /* CONNECT4_AI_H */
