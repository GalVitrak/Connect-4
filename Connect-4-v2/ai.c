/**
 * Connect-4 Game - AI Module
 * Implements multiple AI difficulty levels
 * 
 * Easy:   Random valid moves
 * Medium: Win/block detection + strategic positioning
 * Hard:   Minimax algorithm with alpha-beta pruning
 */

#include "ai.h"
#include "board.h"
#include "input.h"
#include "game.h"
#include <stdlib.h>
#include <time.h>
#include <limits.h>

/**
 * Find a winning move for the specified player
 * Simulates placing a piece in each column and checks if it wins
 * 
 * Parameters:
 *   player - Which player to find winning move for
 * 
 * Returns: GameMove with row/col if winning move found, {-1,-1} otherwise
 */
GameMove find_winning_move(Cell player) {
	/* Try each column */
	for (int col = 0; col < COLS; col++) {
		/* Find the lowest empty row in this column */
		for (int row = ROWS - 1; row >= 0; row--) {
			if (board[row][col] == cell_empty) {
				/* Simulate placing the piece */
				board[row][col] = player;
				GameMove test_move = { row, col };

				/* Check if this move wins */
				if (check_winner(test_move, player) == 1) {
					board[row][col] = cell_empty; /* Undo simulation */
					return test_move; /* Return winning move */
				}

				board[row][col] = cell_empty; /* Undo simulation */
				break; /* Only check lowest position in column */
			}
		}
	}

	/* No winning move found */
	GameMove no_move = { -1, -1 };
	return no_move;
}

/**
 * Count potential threats (2 or 3 in a row) for a given move
 * Used by medium AI to evaluate strategic positions
 * 
 * Parameters:
 *   row, col - Position to evaluate
 *   player   - Which player to evaluate for
 * 
 * Returns: Threat score (higher is better)
 */
int count_threats(int row, int col, Cell player) {
	int threats = 0;
	
	/* Temporarily place piece to test */
	board[row][col] = player;
	
	/* Check horizontal connections */
	int count = 1;
	int temp_col = col;
	/* Count right */
	while (temp_col + 1 < COLS && board[row][temp_col + 1] == player) {
		count++;
		temp_col++;
	}
	/* Count left */
	temp_col = col;
	while (temp_col - 1 >= 0 && board[row][temp_col - 1] == player) {
		count++;
		temp_col--;
	}
	if (count >= 2) threats += count; /* Award points for connections */
	
	/* Check vertical connections */
	count = 1;
	int temp_row = row;
	/* Count down */
	while (temp_row + 1 < ROWS && board[temp_row + 1][col] == player) {
		count++;
		temp_row++;
	}
	/* Count up */
	temp_row = row;
	while (temp_row - 1 >= 0 && board[temp_row - 1][col] == player) {
		count++;
		temp_row--;
	}
	if (count >= 2) threats += count;
	
	board[row][col] = cell_empty; /* Undo test */
	
	return threats;
}

/**
 * Find the best strategic move based on threat analysis
 * Considers piece connections and center column control
 * 
 * Returns: Best move found, or {-1,-1} if none
 */
GameMove find_best_strategic_move(Cell player) {
	int best_score = -1;
	GameMove best_move = {-1, -1};
	
	/* Evaluate each possible move */
	for (int col = 0; col < COLS; col++) {
		for (int row = ROWS - 1; row >= 0; row--) {
			if (board[row][col] == cell_empty) {
				int score = count_threats(row, col, player);
				
				/* Bonus points for center columns (more winning opportunities) */
				if (col == 3) score += 3;      /* Center column */
				else if (col == 2 || col == 4) score += 2; /* Near center */
				else if (col == 1 || col == 5) score += 1; /* Somewhat central */
				
				if (score > best_score) {
					best_score = score;
					best_move.row = row;
					best_move.column = col;
				}
				break; /* Only check lowest position */
			}
		}
	}
	
	return best_move;
}

/**
 * AI Easy difficulty - Random move selection
 * Picks a random valid column
 * 
 * NOTE: Random seed is initialized once in main.c
 * 
 * Returns: Valid random move
 */
GameMove ai_easy_move(void) {
	int column = rand() % 7;
	
	/* Find lowest empty row in chosen column */
	int i;
	int full = 1;
	int row;
	for (i = 5; i >= 0; i--) {
		if (board[i][column] == cell_empty) {
			full = 0;
			row = i;
			break;
		}
	}
	
	/* If column full, try again recursively */
	if (full) {
		return ai_easy_move();
	}
	else {
		GameMove move = { row, column };
		return move;
	}
}

/**
 * AI Medium difficulty - Strategic play
 * Priority: Win > Block opponent > Strategic position > Center preference
 * Includes 30% randomness for unpredictability
 * 
 * NOTE: Random seed is initialized once in main.c
 * 
 * Returns: Selected move
 */
GameMove ai_medium_move(void) {
	/* Priority 1: Take winning move if available */
	GameMove winning_move = find_winning_move(cell_player2);
	if (winning_move.row != -1) {
		return winning_move;
	}

	/* Priority 2: Block opponent's winning move */
	GameMove blocking_move = find_winning_move(cell_player1);
	if (blocking_move.row != -1) {
		return blocking_move;
	}

	/* Priority 3: Make strategic move (70% of the time) */
	GameMove strategic_move = find_best_strategic_move(cell_player2);
	if (strategic_move.row != -1) {
		if (rand() % 100 < 70) { /* 70% chance to take strategic move */
			return strategic_move;
		}
	}

	/* Priority 4: Random center-biased move (for variety) */
	int center_preference[] = {3, 2, 4, 1, 5, 0, 6};
	
	/* Shuffle for randomness */
	for (int i = 0; i < 7; i++) {
		int j = rand() % 7;
		int temp = center_preference[i];
		center_preference[i] = center_preference[j];
		center_preference[j] = temp;
	}
	
	/* Find first valid column from shuffled list */
	for (int i = 0; i < 7; i++) {
		int col = center_preference[i];
		for (int row = ROWS - 1; row >= 0; row--) {
			if (board[row][col] == cell_empty) {
				GameMove move = { row, col };
				return move;
			}
		}
	}

	/* Fallback (shouldn't reach here) */
	return ai_easy_move();
}

/* ==================== HARD AI - MINIMAX ALGORITHM ==================== */

/**
 * Evaluate the board position for a given player
 * 
 * This function assigns a numerical score to the current board state.
 * The score represents how favorable the position is for the specified player.
 * 
 * Scoring System:
 * - Examines all possible 4-cell "windows" (horizontal, vertical, diagonal)
 * - Each window is scored based on its potential to become a win
 * - Positive scores = good for player, Negative scores = good for opponent
 * 
 * Window Scoring:
 *   Player has 4 in window:  +100 (win)
 *   Player has 3 in window:  +5   (strong threat)
 *   Player has 2 in window:  +2   (weak threat)
 *   Opponent has 4 in window: -100 (loss)
 *   Opponent has 3 in window: -4   (must block)
 * 
 * Additional: +3 bonus for each player piece in center column (column 3)
 *            (center control provides more winning opportunities)
 * 
 * Parameters:
 *   player - The player to evaluate the position for (cell_player1 or cell_player2)
 * 
 * Returns: Integer score (higher = better for player, lower = better for opponent)
 */
int evaluate_position(Cell player) {
	int score = 0;
	Cell opponent = (player == cell_player1) ? cell_player2 : cell_player1;
	
	/* Scan all possible 4-cell windows and score them */
	
	/* ============= Horizontal windows ============= */
	/* Check all possible 4-in-a-row combinations horizontally */
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c <= COLS - 4; c++) {  /* Stop at column 3 (leaves room for 4 cells) */
			int player_count = 0;
			int opponent_count = 0;
			int empty_count = 0;
			
			/* Count pieces in this 4-cell window */
			for (int i = 0; i < 4; i++) {
				if (board[r][c + i] == player) player_count++;
				else if (board[r][c + i] == opponent) opponent_count++;
				else empty_count++;
			}
			
			/* Score this window based on pattern */
			/* If opponent has any pieces, player can't win in this window (and vice versa) */
			if (player_count == 4) score += 100;        /* Win */
			else if (player_count == 3 && empty_count == 1) score += 5;  /* One move from win */
			else if (player_count == 2 && empty_count == 2) score += 2;  /* Two moves from win */
			
			if (opponent_count == 4) score -= 100;      /* Opponent win */
			else if (opponent_count == 3 && empty_count == 1) score -= 4; /* Must block next turn */
		}
	}
	
	/* ============= Vertical windows ============= */
	/* Check all possible 4-in-a-row combinations vertically */
	for (int c = 0; c < COLS; c++) {
		for (int r = 0; r <= ROWS - 4; r++) {  /* Stop at row 2 (leaves room for 4 cells) */
			int player_count = 0;
			int opponent_count = 0;
			int empty_count = 0;
			
			/* Count pieces in this 4-cell window */
			for (int i = 0; i < 4; i++) {
				if (board[r + i][c] == player) player_count++;
				else if (board[r + i][c] == opponent) opponent_count++;
				else empty_count++;
			}
			
			/* Score this window */
			if (player_count == 4) score += 100;
			else if (player_count == 3 && empty_count == 1) score += 5;
			else if (player_count == 2 && empty_count == 2) score += 2;
			
			if (opponent_count == 4) score -= 100;
			else if (opponent_count == 3 && empty_count == 1) score -= 4;
		}
	}
	
	/* ============= Positive slope diagonal windows (/) ============= */
	/* Check all possible 4-in-a-row combinations diagonally (bottom-left to top-right) */
	for (int r = 0; r <= ROWS - 4; r++) {      /* Start from bottom rows */
		for (int c = 0; c <= COLS - 4; c++) {  /* Start from left columns */
			int player_count = 0;
			int opponent_count = 0;
			int empty_count = 0;
			
			/* Count pieces in this 4-cell window */
			/* Window goes: (r,c), (r+1,c+1), (r+2,c+2), (r+3,c+3) */
			for (int i = 0; i < 4; i++) {
				if (board[r + i][c + i] == player) player_count++;
				else if (board[r + i][c + i] == opponent) opponent_count++;
				else empty_count++;
			}
			
			/* Score this window */
			if (player_count == 4) score += 100;
			else if (player_count == 3 && empty_count == 1) score += 5;
			else if (player_count == 2 && empty_count == 2) score += 2;
			
			if (opponent_count == 4) score -= 100;
			else if (opponent_count == 3 && empty_count == 1) score -= 4;
		}
	}
	
	/* ============= Negative slope diagonal windows (\) ============= */
	/* Check all possible 4-in-a-row combinations diagonally (top-left to bottom-right) */
	for (int r = 3; r < ROWS; r++) {           /* Start from row 3 and go down */
		for (int c = 0; c <= COLS - 4; c++) {  /* Start from left columns */
			int player_count = 0;
			int opponent_count = 0;
			int empty_count = 0;
			
			/* Count pieces in this 4-cell window */
			/* Window goes: (r,c), (r-1,c+1), (r-2,c+2), (r-3,c+3) */
			for (int i = 0; i < 4; i++) {
				if (board[r - i][c + i] == player) player_count++;
				else if (board[r - i][c + i] == opponent) opponent_count++;
				else empty_count++;
			}
			
			/* Score this window */
			if (player_count == 4) score += 100;
			else if (player_count == 3 && empty_count == 1) score += 5;
			else if (player_count == 2 && empty_count == 2) score += 2;
			
			if (opponent_count == 4) score -= 100;
			else if (opponent_count == 3 && empty_count == 1) score -= 4;
		}
	}
	
	/* Bonus for center column control (more opportunities) */
	/* The center column (column 3) can be part of more winning combinations */
	for (int r = 0; r < ROWS; r++) {
		if (board[r][3] == player) score += 3;
	}
	
	return score;
}

/**
 * Check if the game is over (win or draw)
 * 
 * This function determines if the current board state is a terminal state
 * (game has ended). Used by minimax to know when to stop searching.
 * 
 * Checks two conditions:
 * 1. Does any player have 4-in-a-row? (win)
 * 2. Is the top row full? (draw - no more moves possible)
 * 
 * Returns: 1 if game over, 0 if game continues
 */
int is_game_over(void) {
	/* Check for any winning position */
	/* Test every occupied cell to see if it's part of a win */
	for (int r = 0; r < ROWS; r++) {
		for (int c = 0; c < COLS; c++) {
			if (board[r][c] != cell_empty) {
				GameMove test = {r, c};
				if (check_winner(test, board[r][c])) {
					return 1;  /* Found a winner */
				}
			}
		}
	}
	
	/* Check for draw (top row full) */
	/* If top row is full, no more pieces can be placed anywhere */
	int full = 1;
	for (int c = 0; c < COLS; c++) {
		if (board[0][c] == cell_empty) {
			full = 0;  /* Found an empty spot in top row */
			break;
		}
	}
	
	return full;  /* Returns 1 if board full (draw), 0 if moves available */
}

/**
 * Get all valid moves (columns that aren't full)
 * 
 * Scans the board to find which columns still have space for a piece.
 * A column is valid if its top row (row 0) is empty.
 * 
 * Parameters:
 *   valid_cols - Output array to fill with valid column numbers (0-6)
 *   count      - Output pointer: number of valid columns found
 */
void get_valid_moves(int* valid_cols, int* count) {
	*count = 0;
	for (int c = 0; c < COLS; c++) {
		/* Check if top of column is empty */
		if (board[0][c] == cell_empty) {
			valid_cols[*count] = c;  /* Add this column to valid moves */
			(*count)++;
		}
	}
}

/**
 * Make a move on the board (for minimax simulation)
 * 
 * Places a piece in the specified column, following gravity rules.
 * The piece falls to the lowest available row in that column.
 * 
 * Used during minimax search to simulate potential moves without
 * actually committing them to the real game.
 * 
 * Parameters:
 *   col    - Column to place piece in (0-6)
 *   player - Which player's piece to place
 * 
 * Returns: Row where piece was placed, -1 if column full (shouldn't happen)
 */
int make_move(int col, Cell player) {
	/* Find lowest empty row in this column */
	for (int r = ROWS - 1; r >= 0; r--) {
		if (board[r][col] == cell_empty) {
			board[r][col] = player;  /* Place the piece */
			return r;                /* Return row position */
		}
	}
	return -1;  /* Column was full (error case) */
}

/**
 * Undo a move (for minimax simulation)
 * 
 * Removes a piece that was placed during minimax search.
 * This allows the algorithm to explore different possibilities
 * without affecting the actual game state.
 * 
 * Parameters:
 *   row - Row position of piece to remove
 *   col - Column position of piece to remove
 */
void undo_move(int row, int col) {
	board[row][col] = cell_empty;  /* Remove the piece */
}

/**
 * Minimax algorithm with alpha-beta pruning
 * 
 * This is the core of the Hard AI - a recursive algorithm that looks ahead
 * multiple moves to find the optimal play.
 * 
 * === HOW MINIMAX WORKS ===
 * 
 * The algorithm simulates the game tree:
 * - Maximizing player (AI) tries to MAXIMIZE the score
 * - Minimizing player (opponent) tries to MINIMIZE the score
 * - Recursively explores all possible move sequences up to a depth limit
 * - Assumes both players play optimally
 * 
 * Example with depth 2:
 *   Current position (AI to move)
 *   ?? Try move in column 0
 *   ?  ?? Opponent tries column 0 ? evaluate: -5
 *   ?  ?? Opponent tries column 1 ? evaluate: -10
 *   ?  ?? Opponent tries column 2 ? evaluate: -8
 *   ?  ? Opponent picks worst for AI: -10
 *   ?? Try move in column 1
 *   ?  ?? Opponent tries column 0 ? evaluate: +3
 *   ?  ?? Opponent tries column 1 ? evaluate: +5
 *   ?  ? Opponent picks worst for AI: +3
 *   ?? AI picks best: Column 1 (score +3 is better than -10)
 * 
 * === ALPHA-BETA PRUNING ===
 * 
 * Optimization that skips branches that can't affect the final decision.
 * - Alpha: Best value maximizer has found so far
 * - Beta: Best value minimizer has found so far
 * - If beta <= alpha, remaining branches won't change the decision ? prune
 * 
 * Example:
 *   AI found move A with score +5 (alpha = 5)
 *   Testing move B:
 *     Opponent found counter with score +3
 *     Opponent found counter with score +2
 *     Opponent found counter with score +1
 *     ? No need to check more, move B is already worse than A (pruned)
 * 
 * This can reduce search from O(b^d) to O(b^(d/2)) where:
 *   b = branching factor (7 columns)
 *   d = depth (5 levels)
 * 
 * Parameters:
 *   depth      - How many moves ahead to look (decreases with each level)
 *   alpha      - Best score maximizer can guarantee (starts at -?)
 *   beta       - Best score minimizer can guarantee (starts at +?)
 *   maximizing - 1 if it's the maximizing player's turn, 0 for minimizing
 *   player     - The AI player (the one we're maximizing for)
 * 
 * Returns: The best score achievable from this position
 */
int minimax(int depth, int alpha, int beta, int maximizing, Cell player) {
	Cell opponent = (player == cell_player1) ? cell_player2 : cell_player1;
	
	/* ============= Terminal Conditions ============= */
	/* Stop searching if we've reached the depth limit or the game is over */
	if (depth == 0 || is_game_over()) {
		return evaluate_position(player);  /* Return static evaluation of this position */
	}
	
	/* ============= Get Available Moves ============= */
	int valid_cols[COLS];
	int move_count;
	get_valid_moves(valid_cols, &move_count);
	
	if (move_count == 0) {
		return 0; /* Draw - no moves available */
	}
	
	/* ============= Maximizing Player (AI) ============= */
	/* AI wants to MAXIMIZE the score */
	if (maximizing) {
		int max_eval = INT_MIN;  /* Start with worst possible score */
		
		/* Try each possible move */
		for (int i = 0; i < move_count; i++) {
			int col = valid_cols[i];
			
			/* Simulate making this move */
			int row = make_move(col, player);
			
			/* Recursively evaluate resulting position */
			/* Switch to minimizing (opponent's turn) and decrease depth */
			int eval = minimax(depth - 1, alpha, beta, 0, player);
			
			/* Undo the move to restore board state */
			undo_move(row, col);
			
			/* Track best score found */
			max_eval = (eval > max_eval) ? eval : max_eval;
			
			/* Update alpha (best score maximizer has found) */
			alpha = (alpha > eval) ? alpha : eval;
			
			/* Alpha-beta pruning */
			/* If beta <= alpha, the minimizer won't choose this branch */
			/* because they already have a better option elsewhere */
			if (beta <= alpha) {
				break; /* Beta cutoff - prune remaining branches */
			}
		}
		return max_eval;  /* Return best score AI can achieve */
		
	/* ============= Minimizing Player (Opponent) ============= */
	/* Opponent wants to MINIMIZE the score (bad for AI) */
	} else {
		int min_eval = INT_MAX;  /* Start with worst possible score for minimizer */
		
		/* Try each possible move */
		for (int i = 0; i < move_count; i++) {
			int col = valid_cols[i];
			
			/* Simulate opponent making this move */
			int row = make_move(col, opponent);
			
			/* Recursively evaluate resulting position */
			/* Switch back to maximizing (AI's turn) and decrease depth */
			int eval = minimax(depth - 1, alpha, beta, 1, player);
			
			/* Undo the move to restore board state */
			undo_move(row, col);
			
			/* Track worst score found (worst for AI = best for opponent) */
			min_eval = (eval < min_eval) ? eval : min_eval;
			
			/* Update beta (best score minimizer has found) */
			beta = (beta < eval) ? beta : eval;
			
			/* Alpha-beta pruning */
			/* If beta <= alpha, the maximizer won't choose the parent branch */
			/* because they already have a better option elsewhere */
			if (beta <= alpha) {
				break; /* Alpha cutoff - prune remaining branches */
			}
		}
		return min_eval;  /* Return worst score opponent forces AI to accept */
	}
}

/**
 * AI Hard difficulty - Minimax with alpha-beta pruning
 * 
 * This is the most challenging AI opponent that uses the minimax algorithm
 * to play near-optimally by thinking several moves ahead.
 * 
 * === STRATEGY ===
 * 
 * 1. IMMEDIATE WIN: If AI can win this turn, take it (no need to search)
 * 2. IMMEDIATE BLOCK: If opponent can win next turn, block it (prevent loss)
 * 3. MINIMAX SEARCH: Otherwise, use minimax to find the best move by:
 *    - Looking ahead 5 moves (SEARCH_DEPTH = 5)
 *    - Simulating all possible move sequences
 *    - Assuming opponent also plays optimally
 *    - Choosing the move that gives the best guaranteed outcome
 * 
 * === WHY IT'S STRONG ===
 * 
 * At depth 5, the AI considers:
 * - AI move ? opponent response ? AI response ? opponent response ? AI response
 * - With 7 columns, this could be up to 7^5 = 16,807 positions
 * - Alpha-beta pruning typically reduces this to ~500-2000 positions
 * - This allows the AI to see complex tactics and traps several moves ahead
 * 
 * === EXAMPLE SEARCH ===
 * 
 * Current board with AI to move:
 *   ???????????????
 *   ? . . . . . . .?  Row 0
 *   ? . . . . . . .?  Row 1
 *   ? . . R . . . .?  Row 2
 *   ? . . R Y . . .?  Row 3
 *   ? . Y R Y . . .?  Row 4
 *   ? R Y R Y . . .?  Row 5
 *   ???????????????
 *     0 1 2 3 4 5 6
 * 
 * AI evaluates:
 * - Column 2: Creates threat but opponent blocks ? score: +8
 * - Column 3: Opponent gets 3-in-a-row ? score: -15
 * - Column 4: Creates winning setup in 3 moves ? score: +25 ? BEST!
 * 
 * AI plays column 4 because minimax search revealed it leads to best outcome.
 * 
 * Returns: The optimal GameMove (row and column to play)
 */
GameMove ai_hard_move(void) {
	const int SEARCH_DEPTH = 5; /* Look ahead 5 moves (5 ply) */
	
	/* ============= Priority 1: Take Immediate Win ============= */
	/* If we can win right now, do it - no need to search */
	GameMove winning_move = find_winning_move(cell_player2);
	if (winning_move.row != -1) {
		return winning_move;  /* Immediate win found */
	}
	
	/* ============= Priority 2: Block Immediate Loss ============= */
	/* If opponent can win next turn, we MUST block */
	GameMove blocking_move = find_winning_move(cell_player1);
	if (blocking_move.row != -1) {
		return blocking_move;  /* Block opponent's win */
	}
	
	/* ============= Priority 3: Use Minimax to Find Best Move ============= */
	
	/* Get all valid columns we can play in */
	int valid_cols[COLS];
	int move_count;
	get_valid_moves(valid_cols, &move_count);
	
	int best_score = INT_MIN;  /* Start with worst possible score */
	int best_col = 3;          /* Default to center if all else fails */
	
	/* Evaluate each possible move using minimax */
	for (int i = 0; i < move_count; i++) {
		int col = valid_cols[i];
		
		/* Simulate making this move */
		int row = make_move(col, cell_player2);
		
		/* Recursively evaluate this move's outcome */
		/* Start with opponent's turn (minimizing = 0) */
		/* Use alpha-beta window: [-?, +?] */
		int score = minimax(SEARCH_DEPTH - 1, INT_MIN, INT_MAX, 0, cell_player2);
		
		/* Undo the simulation */
		undo_move(row, col);
		
		/* Track the best move found */
		if (score > best_score) {
			best_score = score;
			best_col = col;
		}
	}
	
	/* ============= Execute the Best Move ============= */
	/* Find the row where the piece will land in the chosen column */
	for (int r = ROWS - 1; r >= 0; r--) {
		if (board[r][best_col] == cell_empty) {
			GameMove move = {r, best_col};
			return move;
		}
	}
	
	/* Fallback (should never reach here) */
	return ai_easy_move();
}

