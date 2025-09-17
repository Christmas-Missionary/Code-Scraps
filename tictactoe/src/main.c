#include "tictactoe.h"
#include <assert.h>
#include <stddef.h>

static_assert(sizeof(char) == 1, "God has left us!"); // sign of the apocalypse if this fails
#ifdef CODE_CHECK
#error "Currently doing static analysis. This error was emitted to prevent object files from being written."
#endif

static inline bool is_char_c(const char chr) {
  assert(chr >= ' ' && chr <= '~' && "Input wasn't handled properly!");
  return chr == 'C' || chr == 'c';
}

static void reset(bool is_x_c[const static 1],
                  bool is_o_c[const static 1],
                  tts_board board[const static 1],
                  tte_letter turn[const static 1],
                  const bool is_do_over) {
  assert(is_o_c != NULL && is_x_c != NULL && board != NULL && turn != NULL && "One of those args ain't there, yo!");
  assert(ttf_validate_board(*board) && "Board does not have valid state!");
  assert((*turn == TTE_LETTER_X || *turn == TTE_LETTER_O) && "Turn letter must be X or O!");
  *board = ttf_new_board();
  *turn = 'X';
  if (is_do_over) {
    *is_x_c = is_char_c(ttfi_getc());
    *is_o_c = is_char_c(ttfi_getc());
  }
  ttfo_print_board(board, true);
}

static ttt_index player_index(const tts_board board[const static 1], const tte_letter who) {
  assert(board != NULL && "Pointer to board is null!");
  assert(ttf_validate_board(*board) && "Board does not have valid state!");
  assert((who == TTE_LETTER_X || who == TTE_LETTER_O) && "Turn letter must be X or O!");
  for (signed char i = 0; i <= TTD_MAX_ITER; i++) { // user input loop
    const char input = ttfi_getc();
    assert(input >= ' ' && input <= '~' && "Input wasn't handled properly!");
    if (input == 'r') {
      return TTD_BOARD_AREA;
    }
    if (input == 't') {
      ttfo_turn(who);
      continue;
    }
    if (input == 'b') {
      ttfo_print_board(board, false);
      continue;
    }
    if (input == 'h') {
      ttfo_plain(TTE_PLAIN_PRINT_HELP);
      continue;
    }
    ttt_index res = ttf_input_to_index(input);
    assert(res <= TTD_BOARD_AREA && "Index is not valid nor an index of none/9!");
    if (res >= TTD_BOARD_AREA) {
      ttfo_plain(TTE_PLAIN_PRINT_NOT_ON_BOARD);
      continue;
    }
    if (ttf_get_board_value(*board, res) != TTE_LETTER_NONE) {
      ttfo_plain(TTE_PLAIN_PRINT_FILLED);
      continue;
    }
    return res;
  }
  return TTD_END_OF_INPUT;
}

int main(void) {
  tte_letter whos_turn = TTE_LETTER_X;
  tts_board board = ttf_new_board();
  ttfo_plain(TTE_PLAIN_PRINT_INPUTS);
  bool is_x_cpu = is_char_c(ttfi_getc());
  bool is_o_cpu = is_char_c(ttfi_getc());
  ttfo_print_board(&board, false);
  for (int i = 0; i < 500000; i++) { // main game loop
    if ((whos_turn == TTE_LETTER_X) ? is_x_cpu : is_o_cpu) {
      *ttfg_board_index(&board, ttfg_cpu_index(&board)) = whos_turn;
    } else {
      const ttt_index index = player_index(&board, whos_turn);
      if (index >= TTD_END_OF_INPUT) {
        return 0;
      }
      if (index == TTD_BOARD_AREA) {
        reset(&is_x_cpu, &is_o_cpu, &board, &whos_turn, false);
        continue;
      }
      *ttfg_board_index(&board, index) = whos_turn;
    }
    ttfo_print_board(&board, false);
    tte_win_line winning_line = ttf_check_for_win_line(board, whos_turn);
    assert(winning_line >= 0 && winning_line < TTE_WIN_LINE_ENUM_SIZE && "winning line is out of range of enum!");
    if (winning_line != TTE_WIN_LINE_NO_WINNER_YET) {
      ttfo_win_line(winning_line, whos_turn == TTE_LETTER_X);
      reset(&is_x_cpu, &is_o_cpu, &board, &whos_turn, true);
      continue;
    }
    whos_turn = (whos_turn == TTE_LETTER_X) ? TTE_LETTER_O : TTE_LETTER_X; // flip between 'X' and 'O'
  }
  return 0;
}
