#include <__random/random_device.h>
#include <array> // release compilations of size (-O3) between C-array and std::array are the same
#include <iostream>
#include "term.hpp"

// 0 = Rock, 1 = Paper, 2 = Scissors
// Rock beats Scissors, Scissors beats Paper, Paper beats Rock.
// 0 > 2, 2 > 1, 1 > 0;

namespace { // file scope "static" funcs

signed char char_to_value(const int chr){
    switch (chr) {
        case 'r': case 'R': return 0;
        case 'p': case 'P': return 1;
        case 's': case 'S': return 2;
        default: return -1;
    }
}

const char * value_to_str(const signed char value){
    switch (value){
        case 0: return "Rock";
        case 1: return "Paper";
        case 2: return "Scissors";
        default: return "";
    }
}

const char * determine_winner(const signed char pvalue, const signed char aivalue){
    constexpr std::array<const char *, 9> strs = {
        "Both tie with Rock!", "Paper beats Rock! You lose!", "Rock beats Scissors! You win!",
        "Paper beats Rock! You win!", "Both tie with Paper!", "Scissors beats Paper! You lose!",
        "Rock beats Scissors! You lose!", "Scissors beats Paper! You win!", "Both tie with Scissors!",
    };

    const auto index = static_cast<size_t>((pvalue * 3) + aivalue);
    return (index < 9) ? strs[index] : "";
}

} // end anon namespace

int main() {
    std::random_device rng {};
    std::uniform_int_distribution<signed char> dist(0, 2);

    const termios old_term = get_termios();
    set_termios(turn_off_flags(old_term));

    while (true){
        std::cout << "Rock, Paper, or Scissors? [r/p/s]\n";
        const signed char pvalue = char_to_value(getchar());
        if (pvalue == -1){
            std::cout << "That's not a valid option!\n";
            continue;
        }
        const signed char aivalue = dist(rng);
        std::cout
        << "Player chooses " << value_to_str(pvalue) << "!\n"
        << "Computer chooses " << value_to_str(aivalue) << "!\n"
        << determine_winner(pvalue, aivalue) << '\n'
        << "Play Again? [y/anything else]\n";
        const int val = getchar();
        if (val != 'y' && val != 'Y'){
            set_termios(old_term);
            return 0;
        }
    }
}
