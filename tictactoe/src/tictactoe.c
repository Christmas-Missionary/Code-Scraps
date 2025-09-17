#include "tictactoe.h"
#include <assert.h>
#include <stdio.h>

// What the board will look like
#define TTD_BOARD_LAYOUT "7|8|9\n-----\n4|5|6\n-----\n1|2|3\n"

// Layout of input to box on board
#define TTD_INPUT_LAYOUT "7|8|9  q|w|e\n-----  -----\n4|5|6  a|s|d\n-----  -----\n1|2|3  z|x|c\n"

// given strlen(TTD_INPUT_LAYOUT)
#define TTD_INPUT_LAYOUT_SIZE 31

typedef unsigned char ttt_byte;

bool ttf_validate_board(const tts_board board) {
  ttt_byte num_of_x = 0;
  ttt_byte num_of_o = 0;
  tte_letter squares[TTD_BOARD_AREA] =
    {board.one, board.two, board.three, board.four, board.five, board.six, board.seven, board.eight, board.nine};
  for (ttt_index i = 0; i < TTD_BOARD_AREA; i++) {
    assert(i < TTD_BOARD_AREA && "Index is out of bounds from the board!");
    const char val = squares[i];
    if (val == 'X') {
      num_of_x++;
    } else if (val == 'O') {
      num_of_o++;
    } else if (val != ' ') {
      return false;
    }
  }
  return (num_of_x == num_of_o) || (num_of_x == (num_of_o + 1));
}

// Returns a copy of a certain value from board given it and an index
// (Copy of valid board with only 'X', 'O', and ' '; index from 0 to 8 inclusive) -> value of individual tte_letter on board
tte_letter ttf_get_board_value(const tts_board board, const ttt_index index) {
  assert(ttf_validate_board(board) && "Board contains invalid state!");
  assert(index < TTD_BOARD_AREA && "Index is out of bounds from the board!");
  switch (index) {
    case 0:
      return board.one;
    case 1:
      return board.two;
    case 2:
      return board.three;
    case 3:
      return board.four;
    case 4:
      return board.five;
    case 5:
      return board.six;
    case 6:
      return board.seven;
    case 7:
      return board.eight;
    case 8:
      return board.nine;
    default:
      return 0;
  }
}

tte_letter * ttfg_board_index(tts_board ptr[const static 1], const ttt_index index) {
  assert(ptr != NULL && "Pointer to board is null!");
  assert(ttf_validate_board(*ptr) && "Board contains invalid state!");
  assert(index < TTD_BOARD_AREA && "Index is out of bounds from the board!");
  switch (index) {
    case 0:
      return &ptr->one;
    case 1:
      return &ptr->two;
    case 2:
      return &ptr->three;
    case 3:
      return &ptr->four;
    case 4:
      return &ptr->five;
    case 5:
      return &ptr->six;
    case 6:
      return &ptr->seven;
    case 7:
      return &ptr->eight;
    case 8:
      return &ptr->nine;
    default:
      return &ptr->one; // I prefer to not have a segfault.
  }
}

tts_board ttf_new_board(void) {
  tts_board res = {TTE_LETTER_NONE,
                   TTE_LETTER_NONE,
                   TTE_LETTER_NONE,
                   TTE_LETTER_NONE,
                   TTE_LETTER_NONE,
                   TTE_LETTER_NONE,
                   TTE_LETTER_NONE,
                   TTE_LETTER_NONE,
                   TTE_LETTER_NONE};
  return res;
}

// possible outputs: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 (out of bounds)
ttt_index ttf_input_to_index(const int32_t input) {
  assert(input >= ' ' && input <= '~' &&
         "These should have been handled by ttfi_getc! Please use ttfi_getc to get input!");
  switch (input) {
    case 'z':
    case '1':
      return 0;
    case 'x':
    case '2':
      return 1;
    case 'c':
    case '3':
      return 2;
    case 'a':
    case '4':
      return 3;
    case 's':
    case '5':
      return 4;
    case 'd':
    case '6':
      return 5;
    case 'q':
    case '7':
      return 6;
    case 'w':
    case '8':
      return 7;
    case 'e':
    case '9':
      return 8;
    default:
      break;
  }
  return TTD_BOARD_AREA;
}

tte_win_line ttf_check_for_win_line(const tts_board board, const tte_letter to_look_for) {
  assert(to_look_for == TTE_LETTER_O || to_look_for == TTE_LETTER_X);
  assert(ttf_validate_board(board) && "Board contains invalid state!");
  if (board.one == to_look_for && board.two == to_look_for && board.three == to_look_for) {
    return TTE_WIN_LINE_ROW_ONE;
  }
  if (board.four == to_look_for && board.five == to_look_for && board.six == to_look_for) {
    return TTE_WIN_LINE_ROW_TWO;
  }
  if (board.seven == to_look_for && board.eight == to_look_for && board.nine == to_look_for) {
    return TTE_WIN_LINE_ROW_THREE;
  }
  if (board.one == to_look_for && board.four == to_look_for && board.seven == to_look_for) {
    return TTE_WIN_LINE_COLUMN_ONE;
  }
  if (board.two == to_look_for && board.five == to_look_for && board.eight == to_look_for) {
    return TTE_WIN_LINE_COLUMN_TWO;
  }
  if (board.three == to_look_for && board.six == to_look_for && board.nine == to_look_for) {
    return TTE_WIN_LINE_COLUMN_THREE;
  }
  if (board.seven == to_look_for && board.five == to_look_for && board.three == to_look_for) {
    return TTE_WIN_LINE_DIA_SLASH;
  }
  if (board.one == to_look_for && board.five == to_look_for && board.nine == to_look_for) {
    return TTE_WIN_LINE_DIA_BACKSLASH;
  }
  if (board.one != TTE_LETTER_NONE && board.two != TTE_LETTER_NONE && board.three != TTE_LETTER_NONE &&
      board.four != TTE_LETTER_NONE && board.five != TTE_LETTER_NONE && board.six != TTE_LETTER_NONE &&
      board.seven != TTE_LETTER_NONE && board.eight != TTE_LETTER_NONE && board.nine != TTE_LETTER_NONE) {
    return TTE_WIN_LINE_NO_WINNER_DRAW;
  }
  return TTE_WIN_LINE_NO_WINNER_YET;
}

// (Any entry from tte_plain_print except TTE_PLAIN_PRINT_ENUM_SIZE) -> The pointer to a string literal
static inline char * ttfg_opttostr(const tte_plain_print opt) {
  assert(opt >= 0 && opt < TTE_PLAIN_PRINT_ENUM_SIZE && "Plain print option is beyond bounds of enum!");
  switch (opt) {
    case TTE_PLAIN_PRINT_INPUTS:
      return "*** TIC-TAC-TOE ***\nEach key corresponds to a spot on the board.\n" TTD_INPUT_LAYOUT
             "\nBefore we get started, type 2 characters to set the status of X and O.\nThe first character is for X, "
             "while the other is for O.\nTyping 'C' turns on the computer for the character, while anything else "
             "leaves it off.\nYou can always type Escape or EOF to quit the game.\nX will go first!\n";
    case TTE_PLAIN_PRINT_NOT_ON_BOARD:
      return "That is not on the board!";
    case TTE_PLAIN_PRINT_FILLED:
      return "This spot has already been filled! Try a different spot.";
    case TTE_PLAIN_PRINT_HELP:
      return "\n" TTD_INPUT_LAYOUT "\n'esc' - quits the game."
             "\n'r' - resets the board."
             "\n't' - prints whose turn it is."
             "\n'b' - prints the current board."
             "\n'h' - prints this.\n";
    case TTE_PLAIN_PRINT_ENUM_SIZE:
      break;
  }
  assert(false && "Plain print option is beyond bounds of enum!");
  return "I don't think the writer of this program knows what he is doing!";
}

void ttfo_plain(const tte_plain_print opt) {
  assert(opt >= 0 && opt < TTE_PLAIN_PRINT_ENUM_SIZE && "Plain print option is beyond bounds of enum!");
  (void)puts(ttfg_opttostr(opt));
}

void ttfo_turn(const tte_letter turn) {
  assert((turn == TTE_LETTER_X || turn == TTE_LETTER_O) && "Turn letter must be X or O!");
  (void)printf("It is %c's turn.\n", turn);
}

void ttfo_win_line(const tte_win_line winning_line, const bool if_is_letter_x) {
  assert(winning_line >= 0 && winning_line < TTE_WIN_LINE_ENUM_SIZE && "winning line is out of range of enum!");
  assert(winning_line != TTE_WIN_LINE_NO_WINNER_YET && "There is no winner yet!");
  if (winning_line == TTE_WIN_LINE_NO_WINNER_DRAW) {
    (void)puts("It's a draw!");
  } else {
    (void)printf("%c wins!\n", (if_is_letter_x) ? TTE_LETTER_X : TTE_LETTER_O);
  }
  (void)puts("Set the status of the 2 characters again!");
}

void ttfo_print_board(const tts_board ptr[const static 1], const bool is_resetting) {
  assert(ptr != NULL && "Pointer to board is null!");
  assert(ttf_validate_board(*ptr) && "Board contains invalid state!");
  char to_print[TTD_INPUT_LAYOUT_SIZE] = TTD_BOARD_LAYOUT;
  for (ttt_index board_index = 0; board_index < TTD_BOARD_AREA; board_index++) {
    assert(board_index < TTD_BOARD_AREA && "Someone messed with the for-loop above me!");
    for (int str_index = 0; str_index < TTD_INPUT_LAYOUT_SIZE && to_print[str_index] != 0; str_index++) {
      assert(board_index < TTD_INPUT_LAYOUT_SIZE && "Someone messed with the for-loop above me!");
      if (to_print[str_index] == (board_index + '1')) {
        to_print[str_index] = ttf_get_board_value(*ptr, board_index);
        break;
      }
    }
  }
  if (is_resetting) {
    (void)puts("The board has been reset!\nX goes first!");
  }
  (void)puts(to_print);
}

void exit(int exit_code);

char ttfi_getc(void) {
  int res = getchar();
  for (signed char i = 0; res < ' ' || res > '~'; i++) {
    if (res == EOF || res == '\x1b' || i > TTD_MAX_ITER) {
      exit(0);
    }
    res = getchar();
  }
  assert(res >= ' ' && res <= '~' && "Input wasn't handled properly!");
  return (char)res;
}

///

typedef unsigned char ttt_split_avail_spots;

static inline tte_letter ttf_find_current_turn(const tts_board board) {
  assert(ttf_validate_board(board) && "Board contains invalid state!");
  ttt_byte num_of_x = 0;
  ttt_byte num_of_o = 0;
  for (ttt_index i = 0; i < TTD_BOARD_AREA; i++) {
    const char val = ttf_get_board_value(board, i);
    if (val == 'X') {
      num_of_x++;
    } else if (val == 'O') {
      num_of_o++;
    } else if (val != ' ') {
      assert(false && "Invalid board state! Someone removed assertions before this line!");
      return 0;
    }
  }
  if (num_of_x == num_of_o) {
    return TTE_LETTER_X;
  }
  if (num_of_x == (num_of_o + 1)) {
    return TTE_LETTER_O;
  }
  assert(false && "Invalid board state! Someone removed assertions before this line!");
  return 0;
}

static inline ttt_index ttf_find_final_spot(const tts_board board) {
  assert(ttf_validate_board(board) && "Board contains invalid state!");
  ttt_index res = TTD_BOARD_AREA;
  for (ttt_index i = 0; i < TTD_BOARD_AREA; i++) {
    if (ttf_get_board_value(board, i) == ' ') {
      if (res <= TTD_BOARD_AREA) {
        return TTD_BOARD_AREA;
      }
      res = i;
    }
  }
  return res;
}

static inline ttt_index ttf_find_winning_move(const tts_board board, const char turn) {
  assert(ttf_validate_board(board) && "Board contains invalid state!");
  assert((turn == TTE_LETTER_X || turn == TTE_LETTER_O) && "Turn letter must be X or O!");
  if ((board.one == ' ' && board.two == turn && board.three == turn) ||
      (board.one == ' ' && board.five == turn && board.nine == turn) ||
      (board.one == ' ' && board.four == turn && board.seven == turn)) {
    return 0;
  }
  if ((board.two == ' ' && board.one == turn && board.three == turn) ||
      (board.two == ' ' && board.five == turn && board.eight == turn)) {
    return 1;
  }
  if ((board.three == ' ' && board.one == turn && board.two == turn) ||
      (board.three == ' ' && board.five == turn && board.seven == turn) ||
      (board.three == ' ' && board.six == turn && board.nine == turn)) {
    return 2;
  }
  if ((board.four == ' ' && board.five == turn && board.six == turn) ||
      (board.four == ' ' && board.one == turn && board.seven == turn)) {
    return 3;
  }
  if ((board.five == ' ' && board.four == turn && board.six == turn) ||
      (board.five == ' ' && board.one == turn && board.nine == turn) ||
      (board.five == ' ' && board.two == turn && board.eight == turn) ||
      (board.five == ' ' && board.three == turn && board.seven == turn)) {
    return 4;
  }
  if ((board.six == ' ' && board.four == turn && board.five == turn) ||
      (board.six == ' ' && board.three == turn && board.nine == turn)) {
    return 5;
  }
  if ((board.seven == ' ' && board.one == turn && board.four == turn) ||
      (board.seven == ' ' && board.five == turn && board.three == turn) ||
      (board.seven == ' ' && board.eight == turn && board.nine == turn)) {
    return 6;
  }
  if ((board.eight == ' ' && board.seven == turn && board.nine == turn) ||
      (board.eight == ' ' && board.two == turn && board.five == turn)) {
    return 7;
  }
  if ((board.nine == ' ' && board.seven == turn && board.eight == turn) ||
      (board.nine == ' ' && board.one == turn && board.five == turn) ||
      (board.nine == ' ' && board.three == turn && board.six == turn)) {
    return 8;
  }
  return TTD_BOARD_AREA;
}

static inline bool ttf_has_one_x(const tts_board board) {
  assert(ttf_validate_board(board) && "Board contains invalid state!");
  ttt_byte res = 0;
  for (ttt_index i = 0; i < TTD_BOARD_AREA; i++) {
    res += ttf_get_board_value(board, i) == 'X';
  }
  return res == 1;
}

#include "rng/rng.h"
#include <time.h>

static inline void ttr_get_and_use_seed(ranxoshi_t state[const static 1]) {
  assert(state != NULL && "Pointer to rng state is null!");
  uint64_t long_one = 0;
  uint64_t long_two = 0;
  {
    struct timespec tis;
    (void)timespec_get(&tis, TIME_UTC);
    long_one = (uint64_t)tis.tv_nsec;
    long_two = (uint64_t)tis.tv_sec;
  }
  ttt_byte seed[RANXOSHI_SEED_SIZE];
  for (ttt_byte i = 0; i < 8; i++) {
    seed[i] = (ttt_byte)(long_one & 255U);
    long_one >>= 8U;
  }
  for (ttt_byte i = 8; i < 16; i++) {
    seed[i] = (ttt_byte)(long_two & 255U);
    long_two >>= 8U;
  }
  time_t cur_time = 0;
  (void)time(&cur_time);
  struct tm cal_time;
  localtime_r(&cur_time, &cal_time);
  seed[16] = (ttt_byte)cal_time.tm_sec;
  seed[17] = (ttt_byte)cal_time.tm_min;
  seed[18] = (ttt_byte)cal_time.tm_hour;
  seed[19] = (ttt_byte)cal_time.tm_mday;
  seed[20] = (ttt_byte)cal_time.tm_mon;
  seed[21] = (ttt_byte)cal_time.tm_year;
  seed[22] = (ttt_byte)cal_time.tm_wday;
  seed[23] = (ttt_byte)((uint32_t)cal_time.tm_year & 255U);
  seed[24] = (ttt_byte)(((uint32_t)cal_time.tm_year & 65280U) >> 8U);
  seed[25] ^= (ttt_byte)cal_time.tm_isdst;
  ranxoshi_seed(state, seed);
  // tm_gmtoff and tm_zone are non-standard, may need to find a better way to generate 32 bytes
}

static inline ttt_index ttr_rand_available_spot(const tts_board ptr[const static 1], const bool emp_on_corners) {
  assert(ptr != NULL && "Pointer to board is null!");
  assert(ttf_validate_board(*ptr) && "Board contains invalid state!");
  ttt_index available_spots[TTD_BOARD_AREA];
  ttt_byte size = 0;
  const ttt_index * to_look_for = (emp_on_corners) ? (ttt_index[4]){0, 2, 6, 8} : (ttt_index[4]){1, 3, 5, 7};
  for (ttt_byte i = 0; i < 4; i++) {
    const ttt_index index = to_look_for[i];
    if (ttf_get_board_value(*ptr, index) == ' ') {
      available_spots[size] = index;
      size++;
    }
  }
  if (size == 0) {
    for (ttt_byte i = 0; i < TTD_BOARD_AREA; i++) {
      if (ttf_get_board_value(*ptr, i) == ' ') {
        available_spots[size] = i;
        size++;
      }
    }
  }
  if (size > 0) {
    assert(size <= TTD_BOARD_AREA && "Size is out of bounds from board!");
    static ranxoshi_t rng_state = {{0, 0, 0, 0}};
    if (*(rng_state.bytes) == 0) {
      ttr_get_and_use_seed(&rng_state);
    }
    return available_spots[ranxoshi_rand(&rng_state) % size]; // Generator
  }
  assert(false && "Board is full!");
  return TTD_BOARD_AREA;
}

// flips the letter between 'X' and 'O'
// (letter of either 'X' and 'O') -> the other letter (either 'X' and 'O')
static inline tte_letter ttf_flip_letter(const tte_letter letter) {
  assert((letter == TTE_LETTER_X || letter == TTE_LETTER_O) && "Letter is not X or O!");
  return (letter == TTE_LETTER_X) ? TTE_LETTER_O : TTE_LETTER_X;
}

ttt_index ttfg_cpu_index(const tts_board ptr[const static 1]) {
  assert(ptr != NULL && "Pointer to board is null!");
  assert(ttf_validate_board(*ptr) && "Board contains invalid state!");
  const char cpu_char = ttf_find_current_turn(*ptr);
  assert((cpu_char == 'X' || cpu_char == 'O') && "CPU char to play is not X nor O!");
  if (ttf_check_for_win_line(*ptr, cpu_char) != TTE_WIN_LINE_NO_WINNER_YET ||
      ttf_check_for_win_line(*ptr, ttf_flip_letter(cpu_char)) != TTE_WIN_LINE_NO_WINNER_YET) {
    assert(false && "There is a winner already!");
    return TTD_BOARD_AREA;
  }
  ttt_index res = (cpu_char == 'X') ? ttf_find_final_spot(*ptr) : TTD_BOARD_AREA;
  if (res < TTD_BOARD_AREA) {
    return res;
  }
  res = ttf_find_winning_move(*ptr, cpu_char);
  if (res < TTD_BOARD_AREA) {
    return res;
  }
  res = ttf_find_winning_move(*ptr, ttf_flip_letter(cpu_char)); // blocking move
  if (res < TTD_BOARD_AREA) {
    return res;
  }
  if (cpu_char == 'X' || ptr->five == 'X') {
    return ttr_rand_available_spot(ptr, true);
  }
  if (ttf_has_one_x(*ptr)) {
    return 4;
  }
  return ttr_rand_available_spot(ptr, false);
}
