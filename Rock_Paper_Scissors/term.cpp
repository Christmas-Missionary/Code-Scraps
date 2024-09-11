#include "term.hpp"

#include <unistd.h> // STDIN_FILENO

termios get_termios(){
    termios res {};
    tcgetattr(STDIN_FILENO, &res);
    return res;
}

termios turn_off_flags(termios term){
    term.c_lflag = 0;
    return term;
}

void set_termios(termios const& term){
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}
