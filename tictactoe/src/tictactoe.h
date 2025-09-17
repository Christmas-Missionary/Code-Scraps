/*
tte - enum
tts - struct
ttt - typedef
ttr - deals with rng
TTD - simple define macro

ttf - pure function, "tic-tac-func"
ttfg - general non-pure function
ttfo - output/printing function
ttfi - input function
*/

#ifndef TICTACTOE_BACKEND_
#define TICTACTOE_BACKEND_

#include <stdbool.h>
#include <stdint.h>

// The maximum amount of iterations for most loops, excluding the main game loop
#define TTD_MAX_ITER 126

typedef unsigned char ttt_index;

// Didn't want 36 bytes for a tts_board, went for a phony enum for 9 bytes, 16 at most
// Do not use ' ', 'X', and 'O' directly, use the macros
typedef char tte_letter;
#define TTE_LETTER_NONE ' '
#define TTE_LETTER_X 'X'
#define TTE_LETTER_O 'O'

// Enum for possible winning lines
typedef enum {
  TTE_WIN_LINE_ROW_ONE,
  TTE_WIN_LINE_ROW_TWO,
  TTE_WIN_LINE_ROW_THREE,
  TTE_WIN_LINE_COLUMN_ONE,
  TTE_WIN_LINE_COLUMN_TWO,
  TTE_WIN_LINE_COLUMN_THREE,
  TTE_WIN_LINE_DIA_SLASH,     /* / */
  TTE_WIN_LINE_DIA_BACKSLASH, /* \ */
  TTE_WIN_LINE_NO_WINNER_YET,
  TTE_WIN_LINE_NO_WINNER_DRAW,
  TTE_WIN_LINE_ENUM_SIZE, // Do not use this
} tte_win_line;

// The struct for the tictactoe board
// I didn't want to do tte_letter board [9] to
//   1. avoid buffering and pointers to the buffer
//   2. There is no instance where the buffer is not 9.
typedef struct {
  tte_letter one;
  tte_letter two;
  tte_letter three;
  tte_letter four;
  tte_letter five;
  tte_letter six;
  tte_letter seven;
  tte_letter eight;
  tte_letter nine;
} tts_board;

// The operator [] for tts_board in C++ terms
// (Pointer to valid board with only 'X', 'O', and ' '; index from 0 to 8 inclusive) -> pointer to individual tte_letter on board
tte_letter * ttfg_board_index(tts_board ptr[static 1], ttt_index index);

// (Pointer to valid board with only 'X', 'O', and ' '; static rand state) -> index the CPU chose
ttt_index ttfg_cpu_index(const tts_board ptr[static 1]);

// 3x3 = 9, used also as return value of no index given bad input.
#define TTD_BOARD_AREA 9

#define TTD_END_OF_INPUT 10

// Returns a board with 9 spaces
tts_board ttf_new_board(void);

// Converts the key input to an index on the board.
// (Any input from ' ' to '~' inclusive) -> 0, 1, 2, 3, 4, 5, 6, 7, and 8 as valid indices, and 9 as no index
ttt_index ttf_input_to_index(int32_t input);

// Checks for the winner, or if there is a draw when all 9 boxes are filled.
// (Copy of board with only 'X', 'O', and ' '; letter is either 'X' or 'O') -> any entry from tte_win_line except TTE_WIN_LINE_ENUM_SIZE
tte_win_line ttf_check_for_win_line(tts_board board, tte_letter to_look_for);

// used as an assertion condition to check for validity of board
bool ttf_validate_board(tts_board board);

// Returns a copy of a certain value from board given it and an index
// (Copy of valid board with only 'X', 'O', and ' '; index from 0 to 8 inclusive) -> value of individual tte_letter on board
tte_letter ttf_get_board_value(tts_board board, ttt_index index);

// Options to print a certain string literal
typedef enum {
  TTE_PLAIN_PRINT_INPUTS,
  TTE_PLAIN_PRINT_NOT_ON_BOARD,
  TTE_PLAIN_PRINT_FILLED,
  TTE_PLAIN_PRINT_HELP,
  TTE_PLAIN_PRINT_ENUM_SIZE, // not to be used directly as input to a function.
} tte_plain_print;

// Prints a plain string literal via puts()
// (Any entry from tte_plain_print except TTE_PLAIN_PRINT_ENUM_SIZE) -> terminal buffer
void ttfo_plain(tte_plain_print opt);

// Prints whos turn it is right now
// (letter of either 'X' and 'O') -> terminal buffer
void ttfo_turn(tte_letter turn);

// Prints who won
// (Any entry from tte_win_line except TTE_WIN_LINE_ENUM_SIZE; boolean of 'X' or not (if not 'X', then 'O')) -> terminal buffer
void ttfo_win_line(tte_win_line winning_line, bool if_is_letter_x);

// prints the current state of the board
// (Pointer to valid board with only 'X', 'O', and ' '; wether or not you reset the game) -> terminal buffer
void ttfo_print_board(const tts_board ptr[static 1], bool is_resetting);

// Gets a filtered value from input that is between ' ' and '~' inclusive
// If it finds EOF, escape, or can't find TDD_MAX_ITER number of valid characters, it calls exit()
// (Some key inputs from user) -> The next valid input between ' ' and '~' inclusive
char ttfi_getc(void);

#endif // TICTACTOE_BACKEND_
