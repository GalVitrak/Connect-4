# Connect-4 Game

A feature-rich implementation of the classic Connect-4 strategy game in C, with multiple AI difficulty levels and comprehensive statistics tracking.

## Features

### Game Modes
- **Player vs Player** - Play against a friend on the same computer
- **Player vs Computer** - Challenge the AI with three difficulty levels:
  - **Easy** - Random but valid moves
  - **Medium** - Strategic play with win/block detection and center preference
  - **Hard** - Advanced AI using Minimax algorithm with alpha-beta pruning

### User Interface
- Colored ASCII board display (Red for Player 1, Yellow for Player 2/Computer)
- Centered text layout for professional appearance
- Cross-platform support (Windows & Unix/Linux)
- Clear numbered menu system
- Input validation with helpful error messages

### Statistics
- Comprehensive tracking of wins/losses across all difficulty levels
- Player vs Player game statistics
- Persistent statistics during runtime

## Technical Implementation

### Code Structure
The project is organized into modular components:

```
Connect-4-v2/
├── main.c          # Entry point
├── board.c/h       # Board management and display
├── game.c/h        # Game logic and win detection
├── ai.c/h          # AI algorithms (Easy, Medium, Hard)
├── input.c/h       # User input handling and validation
├── menu.c/h        # Menu system and console utilities
└── stats.c/h       # Statistics tracking
```

### Key Features

#### AI Implementation
1. **Easy AI**: Random column selection
2. **Medium AI**: 
   - Wins when possible
   - Blocks opponent's winning moves
   - Strategic positioning with center preference
   - 30% randomness for unpredictability
3. **Hard AI**: 
   - Minimax algorithm with alpha-beta pruning
   - 5-move lookahead
   - Position evaluation based on potential 4-in-a-row patterns
   - Optimal play strategy

#### Input Validation
- Handles invalid input (non-numeric, out of range)
- Full column detection
- Clear error messages

#### Win Detection
- Efficient check for:
  - Vertical 4-in-a-row
  - Horizontal 4-in-a-row
  - Diagonal 4-in-a-row (both directions)
- Draw detection (board full)

## Compilation

### Windows (Visual Studio)
Open the `.sln` file and build (F7)

### Windows (MinGW/GCC)
```bash
gcc -o connect4 main.c board.c game.c ai.c input.c menu.c stats.c
```

### Linux/Unix
```bash
gcc -o connect4 main.c board.c game.c ai.c input.c menu.c stats.c
```

## Usage

Run the executable:
```bash
./connect4
```

Follow the on-screen menu:
1. Select game mode (PvP or vs Computer)
2. If vs Computer, choose difficulty
3. Enter column numbers (1-7) to place pieces
4. View statistics from the main menu

## Game Rules

- Board: 6 rows × 7 columns
- Players take turns dropping pieces into columns
- Pieces fall to the lowest available position
- First player to get 4 pieces in a row (vertical, horizontal, or diagonal) wins
- If the board fills up with no winner, the game is a draw

## Code Quality

### Standards Followed
- ✅ Modular design with clear separation of concerns
- ✅ Comprehensive function documentation
- ✅ Header guards on all `.h` files
- ✅ Input validation and error handling
- ✅ English variable and function names
- ✅ Proper code indentation
- ✅ Magic numbers replaced with constants (`WIN_COUNT`, `BOARD_FULL`)

### Requirements Met (מבוא למדעי המחשב)
- ✅ אתחול לוח משחק  - `board_init()`
- ✅ הצגת לוח - `board_print()` with colors
- ✅ מהלך שחקן - Full validation and gravity simulation
- ✅ בדיקת ניצחון - All directions checked
- ✅ בדיקת תיקו - Board full detection
- ✅ תפריט ראשי - Complete menu system
- ✅ 3 רמות קושי - Easy, Medium, Hard
- ✅ פירוק לפונקציות - Modular architecture
- ✅ קריאות קוד - Clear naming and documentation
- ✅ טיפול בקלט לא תקין - Comprehensive validation

## AI Usage Documentation

This project was developed with assistance from GitHub Copilot AI. The AI was primarily used for:

### Areas Where AI Assisted:
- **Logic Guidance**: 
  - Algorithm suggestions for win detection (checking all directions efficiently)
  - Minimax algorithm structure and alpha-beta pruning implementation
  - Strategic positioning heuristics for Medium AI

- **Code Hints**:
  - Cross-platform console clearing (`system("cls")` vs `system("clear")`)
  - ANSI color codes for terminal output
  - Windows console API for detecting terminal width

- **Bug Fixes**:
  - Header guard implementation to prevent multiple definitions
  - Fixing `extern` keyword misuse in variable declarations
  - Resolving linker errors with board array definition
  - Input buffer clearing issues after `scanf()`

- **Code Review**:
  - Identifying missing function documentation
  - Suggesting constant definitions for magic numbers
  - Recommending consistent function signature patterns

### Core Implementation by Developer:
- Overall project architecture and module organization
- Game flow and state management
- Menu system design
- Statistics tracking implementation
- All final design decisions and code integration

The AI served as a coding assistant and knowledge resource, while all architectural decisions, implementation choices, and final code were written and understood by the developer.

## Author

**Gal Vitrak**  
Computer Science Introduction Course Project

## License

Educational use only
