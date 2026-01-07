/**
 * Connect-4 Game - Menu Module
 * Handles main menu, console utilities, and text formatting
 */

#ifndef CONNECT4_MENU_H
#define CONNECT4_MENU_H

/**
 * Display the main menu options
 */
void print_menu(void);

/**
 * Enter the interactive menu loop
 * Blocks until user selects exit option
 */
void menu_loop(void);

/**
 * Clear the console screen
 * Cross-platform implementation (Windows/Unix)
 */
void clear_console(void);

/**
 * Get the current console width
 * Returns width in characters (default 80 if unable to detect)
 */
int get_console_width(void);

/**
 * Print text centered on the console
 * Automatically calculates padding based on console width
 */
void print_centered(const char* text);

#endif /* CONNECT4_MENU_H */