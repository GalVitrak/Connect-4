/**
 * Connect-4 Game - Game Logic Module
 * Implements game flow, turn management, and win detection
 */

#include "board.h"
#include "input.h"
#include "game.h"
#include "menu.h"
#include "ai.h"
#include "stats.h"
#include <stdio.h>

/**
 * Initialize and run a Player vs Player game
 * Two human players take turns until win or draw
 */
void game_init_pvp(void) {
	int running = 1;
	count_game_pvp(); /* Track this game in statistics */
	int count_moves = 0;
	int turn = 0; /* 0 = player 1, 1 = player 2 */
	int winner = 0;
	GameMove move;
	Cell player;
	
	while (running) {
		/* Check for draw (board full) */
		if (count_moves == BOARD_FULL) {
			print_centered("It's a Draw!");
			running = press_enter_to_proceed();
			clear_console();
			break;
		}
		
		/* Determine current player and get their move */
		if (turn == 0) {
			print_centered("Player 1's turn (Red)");
			player = cell_player1;
			turn = 1;
		} else {
			print_centered("Player 2's turn (Yellow)");
			player = cell_player2;
			turn = 0;
		}
		
		/* Get and execute move */
		move = input_player_move();
		board[move.row][move.column] = player;
		clear_console();
		board_print();
		count_moves++;
		
		/* Check for winner (only possible after 7 moves) */
		if (count_moves > 6) {
			winner = check_winner(move, player);
			if(winner == 1){
				if (player == cell_player1) {
					print_centered("🎉 Player 1 (Red) wins! 🎉");
					player_1_win_pvp();
				} else {
					print_centered("🎉 Player 2 (Yellow) wins! 🎉");
					player_2_win_pvp();
				}
				running = press_enter_to_proceed();
				clear_console();
				break;
			}
		}
	}
}

/**
 * Initialize and run Player vs Computer - Easy difficulty
 * Human player vs AI with random move selection
 */
void game_init_easy(void) {
	int running = 1;
	count_game_easy(); /* Track this game in statistics */
	int count_moves = 0;
	int turn = 0; /* 0 = player, 1 = computer */
	int winner = 0;
	Cell player;
	GameMove move;
	
	while (running) {
		/* Check for draw */
		if (count_moves == BOARD_FULL) {
			print_centered("It's a Draw!");
			running = press_enter_to_proceed();
			clear_console();
			break;
		}
		
		/* Alternate between human player and AI */
		if (turn == 0) {
			print_centered("Player 1's turn (Red)");
			player = cell_player1;
			move = input_player_move();
			turn = 1;
		}
		else {
			print_centered("Computer is thinking...");
			player = cell_player2;
			move = ai_easy_move();
			
			/* Place AI's move on board */
			board[move.row][move.column] = player;
			
			/* Show updated board with AI's move */
			clear_console();
			board_print();
			
			/* Announce AI's move */
			char ai_message[100];
			snprintf(ai_message, sizeof(ai_message), 
				"AI placed a piece in Column %d. Your turn!", move.column + 1);
			print_centered(ai_message);
			printf("\n");
			
			turn = 0;
			count_moves++;
			
			/* Check for winner after AI move */
			if (count_moves > 6) {
				winner = check_winner(move, player);
				if (winner == 1) {
					print_centered("Computer Won! Better luck next time!");
					pc_win_easy();
					running = press_enter_to_proceed();
					clear_console();
					break;
				}
			}
			continue; /* Skip the common move execution below */
		}
		
		/* Execute player move and update display */
		board[move.row][move.column] = player;
		clear_console();
		board_print();
		count_moves++;
		
		/* Check for winner after player move */
		if (count_moves > 6) {
			winner = check_winner(move, player);
			if (winner == 1) {
				print_centered("Player (Red) wins!");
				player_1_win_easy();
				running = press_enter_to_proceed();
				clear_console();
				break;
			}
		}
	}
}

/**
 * Initialize and run Player vs Computer - Medium difficulty
 * Human player vs AI with strategic move selection
 */
void game_init_medium(void) {
	int running = 1;
	count_game_medium(); /* Track this game in statistics */
	int count_moves = 0;
	int turn = 0; /* 0 = player, 1 = computer */
	int winner = 0;
	Cell player;
	GameMove move;
	
	while (running) {
		if (count_moves == BOARD_FULL) {
			print_centered("It's a Draw!");
			running = press_enter_to_proceed();
			clear_console();
			break;
		}
		
		if (turn == 0) {
			print_centered("Player 1's turn (Red)");
			player = cell_player1;
			move = input_player_move();
			turn = 1;
		}
		else {
			print_centered("Computer is thinking...");
			player = cell_player2;
			move = ai_medium_move();
			
			/* Place AI's move on board */
			board[move.row][move.column] = player;
			
			/* Show updated board with AI's move */
			clear_console();
			board_print();
			
			/* Announce AI's move */
			char ai_message[100];
			snprintf(ai_message, sizeof(ai_message), 
				"AI placed a piece in Column %d. Your turn!", move.column + 1);
			print_centered(ai_message);
			printf("\n");
			
			turn = 0;
			count_moves++;
			
			/* Check for winner after AI move */
			if (count_moves > 6) {
				winner = check_winner(move, player);
				if (winner == 1) {
					print_centered("Computer Won! Better luck next time!");
					pc_win_medium();
					running = press_enter_to_proceed();
					clear_console();
					break;
				}
			}
			continue; /* Skip the common move execution below */
		}
		
		board[move.row][move.column] = player;
		clear_console();
		board_print();
		count_moves++;
		
		if (count_moves > 6) {
			winner = check_winner(move, player);
			if (winner == 1) {
				print_centered("Player 1 (Red) wins!");
				player_1_win_medium();
				running = press_enter_to_proceed();
				clear_console();
				break;
			}
		}
	}
}

/**
 * Initialize and run Player vs Computer - Hard difficulty
 * Human player vs AI using Minimax algorithm (most challenging)
 */
void game_init_hard(void) {
	int running = 1;
	count_game_hard(); /* Track this game in statistics */
	int count_moves = 0;
	int turn = 0; /* 0 = player, 1 = computer */
	int winner = 0;
	Cell player;
	GameMove move;
	
	while (running) {
		if (count_moves == BOARD_FULL) {
			print_centered("It's a Draw!");
			running = press_enter_to_proceed();
			clear_console();
			break;
		}
		
		if (turn == 0) {
			print_centered("Player 1's turn (Red)");
			player = cell_player1;
			move = input_player_move();
			turn = 1;
		}
		else {
			print_centered("Computer is thinking hard...");
			player = cell_player2;
			move = ai_hard_move();
			
			/* Place AI's move on board */
			board[move.row][move.column] = player;
			
			/* Show updated board with AI's move */
			clear_console();
			board_print();
			
			/* Announce AI's move */
			char ai_message[100];
			snprintf(ai_message, sizeof(ai_message), 
				"AI placed a piece in Column %d. Your turn!", move.column + 1);
			print_centered(ai_message);
			printf("\n");
			
			turn = 0;
			count_moves++;
			
			/* Check for winner after AI move */
			if (count_moves > 6) {
				winner = check_winner(move, player);
				if (winner == 1) {
					print_centered("Computer Won! The AI is too strong!");
					pc_win_hard();
					running = press_enter_to_proceed();
					clear_console();
					break;
				}
			}
			continue; /* Skip the common move execution below */
		}
		
		board[move.row][move.column] = player;
		clear_console();
		board_print();
		count_moves++;
		
		if (count_moves > 6) {
			winner = check_winner(move, player);
			if (winner == 1) {
				print_centered("AMAZING! You beat the Hard AI!");
				player_1_win_hard();
				running = press_enter_to_proceed();
				clear_console();
				break;
			}
		}
	}
}

/**
 * Check for vertical 4-in-a-row from last move
 * 
 * Algorithm:
 * 1. Start counting at 1 (the piece just placed)
 * 2. Count downward from the placed piece (increasing rows)
 * 3. Count upward from the placed piece (decreasing rows)
 * 4. Total count = down + placed piece + up
 * 
 * Example: Piece placed at row 2 in a column with pieces at rows 1,2,3,4
 *   - Count down from row 2: finds pieces at rows 3,4 → adds 2
 *   - Count up from row 2: finds piece at row 1 → adds 1
 *   - Total: 2 (down) + 1 (placed) + 1 (up) = 4 → WIN!
 * 
 * Returns: 1 if win detected, 0 otherwise
 */
int check_vertical(GameMove last_move, Cell player) {
	int temp_row, temp_col;
	temp_col = last_move.column;
	temp_row = last_move.row;
	int count = 1; /* Start with the placed piece */
	
	/* Count downwards (row increases) */
	/* Keep moving down while: within bounds AND next cell matches player */
	while (temp_row + 1 < ROWS && board[temp_row + 1][temp_col] == player) {
		count++;      /* Found another matching piece */
		temp_row++;   /* Move to next row down */
	}
	
	/* Continue counting upwards (row decreases) - DON'T RESET count */
	/* Reset position to starting point before counting in opposite direction */
	temp_row = last_move.row;
	/* Keep moving up while: within bounds AND next cell matches player */
	while (temp_row - 1 >= 0 && board[temp_row - 1][temp_col] == player) {
		count++;      /* Add to existing count */
		temp_row--;   /* Move to next row up */
	}
	
	/* Win if we found 4 or more consecutive pieces */
	return (count >= WIN_COUNT) ? 1 : 0;
}

/**
 * Check for horizontal 4-in-a-row from last move
 * 
 * Algorithm:
 * 1. Start counting at 1 (the piece just placed)
 * 2. Count rightward from the placed piece (increasing columns)
 * 3. Count leftward from the placed piece (decreasing columns)
 * 4. Total count = right + placed piece + left
 * 
 * Example: Piece placed at column 2 with pieces at columns 0,1,2,3
 *   - Count right from col 2: finds piece at col 3 → adds 1
 *   - Count left from col 2: finds pieces at cols 1,0 → adds 2
 *   - Total: 1 (right) + 1 (placed) + 2 (left) = 4 → WIN!
 * 
 * Returns: 1 if win detected, 0 otherwise
 */
int check_horizontal(GameMove last_move, Cell player) {
	int temp_row, temp_col;
	temp_col = last_move.column;
	temp_row = last_move.row;
	int count = 1; /* Start with the placed piece */
	
	/* Count right (column increases) */
	/* Keep moving right while: within bounds AND next cell matches player */
	while (temp_col + 1 < COLS && board[temp_row][temp_col + 1] == player) {
		count++;      /* Found another matching piece */
		temp_col++;   /* Move to next column right */
	}
	
	/* Continue counting left - DON'T RESET count */
	/* Reset position to starting point before counting in opposite direction */
	temp_col = last_move.column;
	/* Keep moving left while: within bounds AND next cell matches player */
	while (temp_col - 1 >= 0 && board[temp_row][temp_col - 1] == player) {
		count++;      /* Add to existing count */
		temp_col--;   /* Move to next column left */
	}
	
	/* Win if we found 4 or more consecutive pieces */
	return (count >= WIN_COUNT) ? 1 : 0;
}

/**
 * Check for diagonal 4-in-a-row from last move
 * 
 * Checks TWO separate diagonals:
 * 1. Positive slope (/) - goes from bottom-left to top-right
 * 2. Negative slope (\) - goes from top-left to bottom-right
 * 
 * Algorithm (for each diagonal):
 * 1. Start counting at 1 (the piece just placed)
 * 2. Count in one direction along the diagonal
 * 3. Count in opposite direction along the same diagonal
 * 4. Check if total >= 4
 * 
 * Example (positive slope /):
 *   Piece at (3,2) with pieces at (2,3), (3,2), (4,1), (5,0)
 *   - Count up-right from (3,2): finds (2,3) → adds 1
 *   - Count down-left from (3,2): finds (4,1), (5,0) → adds 2
 *   - Total: 1 + 1 (placed) + 2 = 4 → WIN!
 * 
 * Returns: 1 if win detected, 0 otherwise
 */
int check_diagonal(GameMove last_move, Cell player) {
	int temp_row, temp_col;
	int count;
	
	/* ============= Check positive slope diagonal (/) ============= */
	/* This diagonal goes from bottom-left to top-right */
	count = 1; /* Start with the placed piece */
	
	/* Count up-right direction (row decreases, column increases) */
	temp_row = last_move.row;
	temp_col = last_move.column;
	/* Move diagonally up-right: row-1, col+1 */
	while (temp_row - 1 >= 0 && temp_col + 1 < COLS && 
	       board[temp_row - 1][temp_col + 1] == player) {
		count++;      /* Found matching piece */
		temp_row--;   /* Move up */
		temp_col++;   /* Move right */
	}
	
	/* Count down-left direction (row increases, column decreases) */
	/* Reset to starting position, DON'T reset count */
	temp_row = last_move.row;
	temp_col = last_move.column;
	/* Move diagonally down-left: row+1, col-1 */
	while (temp_row + 1 < ROWS && temp_col - 1 >= 0 && 
	       board[temp_row + 1][temp_col - 1] == player) {
		count++;      /* Add to existing count */
		temp_row++;   /* Move down */
		temp_col--;   /* Move left */
	}
	
	/* Check if we found 4 in this diagonal */
	if (count >= WIN_COUNT) return 1;
	
	/* ============= Check negative slope diagonal (\) ============= */
	/* This diagonal goes from top-left to bottom-right */
	count = 1; /* Reset for new diagonal - this is a different line */
	
	/* Count up-left direction (row decreases, column decreases) */
	temp_row = last_move.row;
	temp_col = last_move.column;
	/* Move diagonally up-left: row-1, col-1 */
	while (temp_row - 1 >= 0 && temp_col - 1 >= 0 && 
	       board[temp_row - 1][temp_col - 1] == player) {
		count++;      /* Found matching piece */
		temp_row--;   /* Move up */
		temp_col--;   /* Move left */
	}
	
	/* Count down-right direction (row increases, column increases) */
	/* Reset to starting position, DON'T reset count */
	temp_row = last_move.row;
	temp_col = last_move.column;
	/* Move diagonally down-right: row+1, col+1 */
	while (temp_row + 1 < ROWS && temp_col + 1 < COLS && 
	       board[temp_row + 1][temp_col + 1] == player) {
		count++;      /* Add to existing count */
		temp_row++;   /* Move down */
		temp_col++;   /* Move right */
	}
	
	/* Check if we found 4 in this diagonal */
	return (count >= WIN_COUNT) ? 1 : 0;
}

/**
 * Check if the last move resulted in a win
 * 
 * This is the main win-checking function that tests all possible win conditions.
 * It only needs to check from the last move position because:
 * - A win can only occur on the turn a piece is placed
 * - The winning 4-in-a-row must include the piece that was just placed
 * 
 * Efficiency: O(1) - each direction check is bounded by WIN_COUNT (4)
 * 
 * Parameters:
 *   last_move - The move that was just played (row and column)
 *   player    - Which player made the move (cell_player1 or cell_player2)
 * 
 * Returns: 1 if winner found, 0 otherwise
 */
int check_winner(GameMove last_move, Cell player) {
	/* Check all win conditions - return immediately if any wins */
	if (check_vertical(last_move, player)) return 1;    /* Vertical line */
	if (check_horizontal(last_move, player)) return 1;  /* Horizontal line */
	if (check_diagonal(last_move, player)) return 1;    /* Both diagonals */
	
	return 0; /* No win detected in any direction */
}