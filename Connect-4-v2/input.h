/**
 * Connect-4 Game - Input Module
 * Handles all user input with validation and error handling
 */

#ifndef CONNECT4_INPUT_H
#define CONNECT4_INPUT_H

typedef struct {
	int row;
	int column;
} GameMove;

/**
 * Get player's column choice and validate
 * Calculates row position based on gravity
 * Returns the move with calculated row position
 */
GameMove input_player_move(void);

/**
 * Wait for user to press Enter before proceeding
 * Clears input buffer and waits for Enter key
 * Returns 0 to signal game loop exit
 */
int press_enter_to_proceed(void);

/**
 * Get integer input within specified range
 * Handles invalid input and out-of-range values
 * Returns valid integer within [low, high] range
 */
int input_player_int_range(int low, int high);

#endif /* CONNECT4_INPUT_H */
