/**
 * Connect-4 Game - Input Module Implementation
 * Handles all user input with validation and error handling
 */

#include <stdio.h>
#include <string.h>
#include "board.h"
#include "input.h"
#include "menu.h"

/**
 * Get integer input within specified range
 * Handles invalid input (non-numeric) and out-of-range values
 * Automatically centers the input prompt on the console
 * 
 * Parameters:
 *   low  - Minimum valid value (inclusive)
 *   high - Maximum valid value (inclusive)
 * Returns: Valid integer within [low, high] range
 */
int input_player_int_range(int low, int high) {
	char prompt[100];
	snprintf(prompt, sizeof(prompt), "Please select an option (%d-%d): ", low, high);
	
	/* Calculate padding to center the prompt */
	int console_width = get_console_width();
	int prompt_len = strlen(prompt);
	int padding = (console_width - prompt_len - 5) / 2;
	
	/* Print centered prompt */
	if (padding > 0) {
		for (int i = 0; i < padding; i++) {
			printf(" ");
		}
	}
	printf("%s", prompt);
	
	int value;
	while (1) {
		int result = scanf("%d", &value);
		
		/* Handle non-numeric input */
		if (result != 1) {
			int c;
			while ((c = getchar()) != '\n' && c != EOF); /* Clear input buffer */
			
			print_centered("Invalid input.");
			if (padding > 0) {
				for (int i = 0; i < padding; i++) {
					printf(" ");
				}
			}
			printf("Please enter a number (%d-%d): ", low, high);
			continue;
		}
		
		/* Handle out-of-range input */
		if (value < low || value > high) {
			print_centered("Input out of range.");
			if (padding > 0) {
				for (int i = 0; i < padding; i++) {
					printf(" ");
				}
			}
			printf("Please enter a number (%d-%d): ", low, high);
			continue;
		}
		
		/* Valid input received */
		break;
	}
	printf("\n");
	return value;
}

/**
 * Get player's column choice and validate
 * Automatically calculates where the piece will land (gravity simulation)
 * Recursively prompts if column is full
 * 
 * Returns: GameMove struct with row and column where piece will be placed
 */
GameMove input_player_move(void) {
	/* Get column choice (convert from 1-based to 0-based index) */
	int column = input_player_int_range(1, 7) - 1;
	
	/* Find the lowest empty row in the chosen column (gravity) */
	int i;
	int full = 1;
	int row;
	for (i = ROWS - 1; i >= 0; i--) {
		if (board[i][column] == cell_empty) {
			full = 0;
			row = i;
			break;
		}
	}
	
	/* If column is full, ask for a different column */
	if (full) {
		print_centered("Column full, please choose another column");
		return input_player_move(); /* Recursive call */
	} else {
		GameMove move = { row, column };
		return move;
	}
}

/**
 * Wait for user to press Enter before proceeding
 * Clears input buffer before and after waiting
 * Used to pause before returning to menu
 * 
 * Returns: 0 (signals game loop to exit)
 */
int press_enter_to_proceed(void) {
	/* Clear any remaining input in buffer */
	int c;
	while ((c = getchar()) != '\n' && c != EOF);

	print_centered("Press Enter to return to main menu...");
	getchar(); /* Wait for Enter key */

	return 0;
}