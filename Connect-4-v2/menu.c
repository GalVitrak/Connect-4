/**
 * Connect-4 Game - Menu Module Implementation
 * Handles main menu navigation and console utilities
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "input.h"
#include "board.h"
#include "game.h"
#include "stats.h"

#ifdef _WIN32
#include <windows.h>
#endif

/**
 * Clear the console screen
 * Cross-platform implementation using system calls
 */
void clear_console(void) {
#ifdef _WIN32
	system("cls");
#else
	system("clear");
#endif
}

/**
 * Get the current console width
 * Used for centering text output
 * 
 * Returns: Console width in characters (default 80 if detection fails)
 */
int get_console_width(void) {
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int width = 80;
	
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
		width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	}
	return width;
#else
	return 80; /* Default for Unix/Linux */
#endif
}

/**
 * Print text centered on the console
 * Automatically calculates padding based on console width
 * 
 * Parameters:
 *   text - String to print (should not contain newlines)
 */
void print_centered(const char* text) {
	int console_width = get_console_width();
	int text_length = strlen(text);
	int padding = (console_width - text_length) / 2;
	
	/* Print left padding */
	if (padding > 0) {
		for (int i = 0; i < padding; i++) {
			printf(" ");
		}
	}
	printf("%s\n", text);
}

/**
 * Display the main menu options
 * Shows all available game modes and features
 */
void print_menu(void) {
	print_centered("=== Connect 4 ===");
	print_centered("1. Player vs Player");
	print_centered("2. Player vs Computer");
	print_centered("3. Statistics");
	print_centered("4. Exit");
	printf("\n");
}

/**
 * Main menu loop
 * Handles user navigation between different game modes
 * Runs until user selects exit option
 */
void menu_loop(void) {
	int choice;
	int running = 1;
	int display = 0;
	
	while (running) {
		print_menu();
		choice = input_player_int_range(1, 4);

		switch (choice) {
		case 1: /* Player vs Player */
			clear_console();
			print_centered("Starting Player vs Player");
			printf("\n");
			board_init();
			board_print();
			game_init_pvp();
			break;

		case 2: /* Player vs Computer */
			clear_console();
			print_centered("Starting Player vs Computer");
			print_centered("Choose Difficulty");
			print_centered("1. Easy");
			print_centered("2. Medium");
			print_centered("3. Hard");
			print_centered("4. Back to menu");
			printf("\n");
			
			int diff = input_player_int_range(1, 4);
			switch (diff) {
			case 1: /* Easy AI */
				clear_console();
				print_centered("Starting Player vs Computer - Easy Difficulty");
				printf("\n");
				board_init();
				board_print();
				game_init_easy();
				break;

			case 2: /* Medium AI */
				clear_console();
				print_centered("Starting Player vs Computer - Medium Difficulty");
				printf("\n");
				board_init();
				board_print();
				game_init_medium();
				break;

			case 3: /* Hard AI */
				clear_console();
				print_centered("Starting Player vs Computer - Hard Difficulty");
				print_centered("Warning: This AI is very challenging!");
				printf("\n");
				board_init();
				board_print();
				game_init_hard();
				break;

			case 4:
				clear_console();
				diff = 0;
				break;
			}
			break;

		case 3: /* Statistics */
			display = 1;
			while (display) {
				clear_console();
				print_stats();
				display = press_enter_to_proceed();
				clear_console();
			}
			break;

		case 4: /* Exit */
			clear_console();
			print_centered("Thanks for playing!");
			print_centered("Goodbye!");
			running = 0;
			break;
		}
	}
}

