#pragma once

#include <termios.h> // termios, TCSANOW, ECHO, ICANON

// returns the current settings of the terminal.
[[nodiscard]]
termios get_termios();

// returns a terminal with local flags off, You don't see what you type + getchar by char.
[[nodiscard, gnu::const]]
termios turn_off_flags(termios term);

// sets the terminal settings, use this to reset with old settings.
void set_termios(termios const& term);
