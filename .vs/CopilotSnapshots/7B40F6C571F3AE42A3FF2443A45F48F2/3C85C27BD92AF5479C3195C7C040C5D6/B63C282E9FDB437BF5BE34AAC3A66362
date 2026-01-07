/**
 * Connect-4 Game - Board Module Implementation
 * Handles board initialization and display with ANSI colors
 */

#include <stdio.h>
#include <stdlib.h>
#include "board.h"

#ifdef _WIN32
#include <windows.h>
#endif

#define ROWS 6
#define COLS 7

/* Global board array - accessible from all modules */
Cell board[ROWS][COLS];

/**
 * Initialize the board to empty state
 * Called at the start of each new game
 */
void board_init(void) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            board[r][c] = cell_empty;
        }
    }
}

#ifdef _WIN32
/**
 * Enable ANSI color support on Windows 10+ consoles
 * Only needs to be called once - uses static flag to prevent repeated calls
 */
static void enable_ansi_colors_once(void)
{
    static int tried = 0;
    if (tried) return;
    tried = 1;

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut == INVALID_HANDLE_VALUE) return;

    DWORD mode = 0;
    if (!GetConsoleMode(hOut, &mode)) return;

    /* Enable virtual terminal processing for ANSI escape codes */
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, mode);
}
#else
/* No-op on Unix/Linux - ANSI colors work by default */
static void enable_ansi_colors_once(void) { }
#endif

/**
 * Get the current console/terminal width in characters
 * Returns: width in characters, defaults to 80 if detection fails
 */
static int get_console_width(void) {
#ifdef _WIN32
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int width = 80;
	
	if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
		width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	}
	return width;
#else
	return 80; /* Default for Unix/Linux - could use ioctl for detection */
#endif
}

/**
 * Display the board with colors and formatting
 * Uses ANSI escape codes for colored pieces:
 * - Red (X) for Player 1
 * - Yellow (O) for Player 2
 * - Gray (.) for empty cells
 */
void board_print(void) {
    enable_ansi_colors_once();

    /* ANSI color escape codes */
    const char* col_red = "\x1b[31m";
    const char* col_yellow = "\x1b[33m";
    const char* col_gray = "\x1b[90m";
    const char* col_reset = "\x1b[0m";

    /* Calculate centering */
    int board_width = 32; /* Approximate width of the board in characters */
    int console_width = get_console_width();
    int left_padding = (console_width - board_width) / 2;

    /* Print column numbers (1-7) */
    for (int i = 0; i < left_padding; i++) printf(" ");
    printf("   ");
    for (int c = 0; c < COLS; ++c) {
        printf("  %d ", c + 1);
    }
    printf("\n");

    /* Top border */
    for (int i = 0; i < left_padding; i++) printf(" ");
    printf("   +");
    for (int c = 0; c < COLS; ++c) printf("---+");
    printf("\n");

    /* Print each row */
    for (int r = 0; r < ROWS; ++r) {
        /* Row number and left border */
        for (int i = 0; i < left_padding; i++) printf(" ");
        printf("%2d |", r + 1);
        
        /* Print each cell in the row */
        for (int c = 0; c < COLS; ++c) {
            Cell v = board[r][c];
            if (v == cell_player1) {
                printf(" %sX%s |", col_red, col_reset);
            } else if (v == cell_player2) {
                printf(" %sO%s |", col_yellow, col_reset);
            } else {
                printf(" %s.%s |", col_gray, col_reset);
            }
        }
        printf("\n");

        /* Row separator */
        for (int i = 0; i < left_padding; i++) printf(" ");
        printf("   +");
        for (int c = 0; c < COLS; ++c) printf("---+");
        printf("\n");
    }
}