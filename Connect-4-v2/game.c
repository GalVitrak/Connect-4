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
					print_centered("Player 1 (Red) wins!");
					player_1_win_pvp();
				} else {
					print_centered("Player 2 (Yellow) wins!");
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
			turn = 0;
		}
		
		/* Execute move and update display */
		board[move.row][move.column] = player;
		clear_console();
		board_print();
		count_moves++;
		
		/* Check for winner */
		if (count_moves > 6) {
			winner = check_winner(move, player);
			if (winner == 1) {
				if (player == cell_player1) {
					print_centered("Player (Red) wins!");
					player_1_win_easy();
				}
				else {
					print_centered("Computer Won! Better luck next time!");
					pc_win_easy();
				}
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
			turn = 0;
		}
		
		board[move.row][move.column] = player;
		clear_console();
		board_print();
		count_moves++;
		
		if (count_moves > 6) {
			winner = check_winner(move, player);
			if (winner == 1) {
				if (player == cell_player1) {
					print_centered("Player 1 (Red) wins!");
					player_1_win_medium();
				}
				else {
					print_centered("Computer Won! Better luck next time!");
					pc_win_medium();
				}
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
			turn = 0;
		}
		
		board[move.row][move.column] = player;
		clear_console();
		board_print();
		count_moves++;
		
		if (count_moves > 6) {
			winner = check_winner(move, player);
			if (winner == 1) {
				if (player == cell_player1) {
					print_centered("AMAZING! You beat the Hard AI!");
					player_1_win_hard();
				}
				else {
					print_centered("Computer Won! The AI is too strong!");
					pc_win_hard();
				}
				running = press_enter_to_proceed();
				clear_console();
				break;
			}
		}
	}
}

/**
 * Check for vertical 4-in-a-row from last move
 * Checks both upward and downward from the placed piece
 * 
 * Returns: 1 if win detected, 0 otherwise
 */
int check_vertical(GameMove last_move, Cell player) {
	int temp_row, temp_col;
	temp_col = last_move.column;
	temp_row = last_move.row;
	int count = 1;
	
	/* Check upwards */
	while (temp_row + 1 < ROWS && board[temp_row + 1][temp_col] == player && count < WIN_COUNT) {
		count++;
		temp_row++;
	}
	if (count == WIN_COUNT) {
		return 1;
	}
	
	/* Reset and check downwards */
	temp_row = last_move.row;
	count = 1;
	while (temp_row - 1 >= 0 && board[temp_row - 1][temp_col] == player && count < WIN_COUNT) {
		count++;
		temp_row--;
	}
	
	return (count == WIN_COUNT) ? 1 : 0;
}

/**
 * Check for horizontal 4-in-a-row from last move
 * Checks both left and right from the placed piece
 * 
 * Returns: 1 if win detected, 0 otherwise
 */
int check_horizontal(GameMove last_move, Cell player) {
	int temp_row, temp_col;
	temp_col = last_move.column;
	temp_row = last_move.row;
	int count = 1;
	
	/* Check right */
	while (temp_col + 1 < COLS && board[temp_row][temp_col + 1] == player && count < WIN_COUNT) {
		count++;
		temp_col++;
	}
	if (count == WIN_COUNT) {
		return 1;
	}
	
	/* Reset and check left */
	temp_col = last_move.column;
	count = 1;
	while (temp_col - 1 >= 0 && board[temp_row][temp_col - 1] == player && count < WIN_COUNT) {
		count++;
		temp_col--;
	}
	
	return (count == WIN_COUNT) ? 1 : 0;
}

/**
 * Check for diagonal 4-in-a-row from last move
 * Checks all four diagonal directions from the placed piece
 * 
 * Returns: 1 if win detected, 0 otherwise
 */
int check_diagonal(GameMove last_move, Cell player) {
	int temp_row, temp_col;
	temp_col = last_move.column;
	temp_row = last_move.row;
	int count = 1;
	
	/* Check positive slope diagonal (bottom-left to top-right) */
	/* First direction: top-right */
	while (temp_row + 1 < ROWS && temp_col + 1 < COLS && board[temp_row + 1][temp_col + 1] == player && count < WIN_COUNT) {
		count++;
		temp_row++;
		temp_col++;
	}
	if (count == WIN_COUNT) return 1;
	
	/* Reset and check opposite: bottom-left */
	temp_row = last_move.row;
	temp_col = last_move.column;
	count = 1;
	while (temp_row - 1 >= 0 && temp_col - 1 >= 0 && board[temp_row - 1][temp_col - 1] == player && count < WIN_COUNT) {
		count++;
		temp_row--;
		temp_col--;
	}
	if (count == WIN_COUNT) return 1;
	
	/* Check negative slope diagonal (top-left to bottom-right) */
	/* Reset and check: top-left */
	temp_row = last_move.row;
	temp_col = last_move.column;
	count = 1;
	while (temp_row + 1 < ROWS && temp_col - 1 >= 0 && board[temp_row + 1][temp_col - 1] == player && count < WIN_COUNT) {
		count++;
		temp_row++;
		temp_col--;
	}
	if (count == WIN_COUNT) return 1;
	
	/* Reset and check opposite: bottom-right */
	temp_row = last_move.row;
	temp_col = last_move.column;
	count = 1;
	while (temp_row - 1 >= 0 && temp_col + 1 < COLS && board[temp_row - 1][temp_col + 1] == player && count < WIN_COUNT) {
		count++;
		temp_row--;
		temp_col++;
	}
	
	return (count == WIN_COUNT) ? 1 : 0;
}

/**
 * Check if the last move resulted in a win
 * Tests all possible win conditions: vertical, horizontal, and diagonal
 * 
 * Parameters:
 *   last_move - The move that was just played
 *   player    - Which player made the move
 * 
 * Returns: 1 if winner found, 0 otherwise
 */
int check_winner(GameMove last_move, Cell player) {
	/* Check all win conditions */
	if (check_vertical(last_move, player)) return 1;
	if (check_horizontal(last_move, player)) return 1;
	if (check_diagonal(last_move, player)) return 1;
	
	return 0; /* No win detected */
}