#define _LIBCPP_REMOVE_TRANSITIVE_INCLUDES

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <utility>

#include "typedefs.hpp"

// https://wiki.froth.zone/wiki/ANSI_escape_code?lang=en#OSC_(Operating_System_Command)_sequences

constexpr std::vector<int32_t> get_new_board(const int32_t num_of_rings){
    std::vector<int32_t> res(static_cast<uint64_t>(num_of_rings) * 3U);
    int32_t to_place = 1;
    for (auto i = res.begin(); to_place <= num_of_rings; to_place++, std::advance(i, 1)){
        *i = to_place;
    }
    return res;
}

constexpr std::vector<int32_t> get_finished_board(const int32_t num_of_rings){
    std::vector<int32_t> res(static_cast<uint64_t>(num_of_rings) * 3U);
    int32_t to_place = num_of_rings;
    for (auto i = res.rbegin(); to_place > 0; to_place--, std::advance(i, 1)){
        *i = to_place;
    }
    return res;
}

constexpr std::string format_board(std::vector<int32_t> const& board, const int32_t num_of_rings){
    constexpr uchar random_reserve_factor = 15;

    std::string res;
    res.reserve(static_cast<uint64_t>(num_of_rings) * random_reserve_factor);
    for (int32_t i = 0; i < num_of_rings; i++){
        for (int32_t j = 0; j < num_of_rings * 3; j += num_of_rings){
            const int32_t value = board[static_cast<uint64_t>(j + i)];
            if (value == 0){
                res += std::to_string(value) + ' ';
            }
            else if ((value & 1) == 0){
                res += "\x1b[92m" + std::to_string(value) + "\x1b[0m ";
            }
            else { // if ((value & 1) == 1)
                res += "\x1b[91m" + std::to_string(value) + "\x1b[0m ";
            }
        }
        res.pop_back();
        res.push_back('\n');
    }
    res += "\x1b[93m-----\n1 2 3\n\x1b[0m";
    return res;
}

constexpr int64_t board_dir_from_where(const int32_t where){
    switch (where) {
        case '1': return -2;
        case '2': return -1;
        case '3': return 1;
        case '4': return 2;
        default: return 0;
    }
}

struct index_pair {
    uchar x;
    uchar y;
};

constexpr index_pair find_valid_move(std::vector<int32_t> const& board, const int32_t num_of_rings, const int32_t value, const int32_t where_to_go){
    if (value < 0 || value > 9 || where_to_go < '1' || where_to_go > '4'){
        return {};
    }
    const auto begin = board.begin();
    const auto end = board.end();

    auto iter = std::find(begin, end, value);
    if ((iter != begin && *(iter - 1) != 0 && iter != (begin + num_of_rings) && iter != (begin + num_of_rings * 2L))){
        return {};
    }

    index_pair res{};
    res.x = static_cast<uchar>(std::distance(begin, iter));

    iter += board_dir_from_where(where_to_go) * num_of_rings;
    if (iter >= end || iter < begin){
        return {};
    }

    while (*iter == 0 && std::distance(begin, iter) % num_of_rings != num_of_rings - 1){
        ++iter;
    }
    while (*iter != 0){
        --iter;
    }
    if (iter + 1 < end && *(iter + 1) < value && *(iter + 1) != 0){
        return {};
    }
    res.y = static_cast<uchar>(std::distance(begin, iter));
    return res;
}

consteval const char * vietnamese_flag(){
    return
"\x1b[91mRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR\n\
RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR\n\
RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR\n\
RRRRRRRRRRRRRRRR\x1b[93mY\x1b[91mRRRRRRRRRRRRRRRR\n\
RRRRRRRRRRRRRRR\x1b[93mYYY\x1b[91mRRRRRRRRRRRRRRR\n\
RRRRRRRRRRR\x1b[93mYYYYYYYYYYY\x1b[91mRRRRRRRRRRR\n\
RRRRRRRRRRRR\x1b[93mYYYYYYYYY\x1b[91mRRRRRRRRRRRR\n\
RRRRRRRRRRRRR\x1b[93mYYYYYYY\x1b[91mRRRRRRRRRRRRR\n\
RRRRRRRRRRR\x1b[93mYYYYYYYYYYY\x1b[91mRRRRRRRRRRR\n\
RRRRRRRRRRR\x1b[93mYY\x1b[91mRRRRRRR\x1b[93mYY\x1b[91mRRRRRRRRRRR\n\
RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR\n\
RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR\n\
RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR\n\x1b[0m";
}

consteval const char * south_vietnamese_flag(){
    return
"\x1b[93mYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY\n\
YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY\n\
YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY\n\
YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY\n\
\x1b[91mRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR\n\
\x1b[93mYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY\n\
\x1b[91mRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR\n\
\x1b[93mYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY\n\
\x1b[91mRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR\n\
\x1b[93mYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY\n\
YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY\n\
YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY\n\
YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY\n\x1b[0m";
}

enum TODO: char {PROCEED, CONTINUE, QUIT, NEW_BOARD, NEW_GAME, PRINT_MOVES};

struct instruction {
    TODO to_do;
    std::string to_print;
};

instruction size_to_instructions(std::string const& full_input){
    constexpr uint64_t s_viet_str_size = 13;
    constexpr uint64_t viet_str_size = 7;
    const uint64_t size = full_input.size();
    if (size == 0){
        return {CONTINUE, "\x1b[91mPlease type something.\x1b[0m\n"};
    }
    if (size == 1){
        if (full_input[0] == 'q' || full_input[0] == 'Q'){
            return {QUIT, ""};
        }
        if (full_input[0] == 'r' || full_input[0] == 'R'){
            return {NEW_BOARD, "\x1b[91mboard resetted with the same number of rings.\x1b[0m\n"};
        }
        return {CONTINUE, "\x1b[93mA move must require 2 numbers:\nthe first from 1 to 9, and the second from 1 to 4. No spaces.\x1b[0m\n"};
    }
    if (size >= s_viet_str_size && full_input.substr(0, s_viet_str_size) == "South Vietnam"){
        return {CONTINUE, south_vietnamese_flag()};
    }
    if (size >= viet_str_size && full_input.substr(0, viet_str_size) == "Vietnam"){
        return {CONTINUE, vietnamese_flag()};
    }
    if (size >= 5 && full_input.substr(0, 5) == "moves"){
        return {PRINT_MOVES, ""};
    }
    if ((size == 2) && (full_input[0] == 'r' || full_input[0] == 'R') && full_input[1] >= '1' && full_input[1] <= '9'){
        std::string res = "\x1b[91mBoard resetted with X rings.\x1b[0m\n"; // do not change this.
        res[25] = full_input[1];
        if (full_input[1] == '1'){
            res.erase(31, 1);
        }
        return {NEW_GAME, res};
    }
    return {PROCEED, ""};
}

/*
THE FUNCTIONS ABOVE ARE "PURE". THESE HAVE NO OUTSIDE SIDE EFFECTS.
THE FUNCTIONS BELOW ARE "NOT PURE". THESE DEAL WITH IO AND VARIABLES FROM main().
*/

// #include <random>

int32_t main(){
    // std::random_device rd;
    // std::mt19937 gen {rd()};
    // auto one = std::uniform_int_distribution<int>(1, 9);
    // auto two = std::uniform_int_distribution<int>('1', '4');

    constexpr uint64_t comfortable_input_reserve = 15;
    std::string full_input{};
    full_input.reserve(comfortable_input_reserve);

    uint64_t number_of_moves = 0;

    std::cout <<
"Hello! Welcome to the Tower of \x1b[93mH\x1b[92ma\x1b[91mn\x1b[93mo\x1b[92mi\x1b[91m!\n\
\x1b[93mThere are a few rules:\n\
\x1b[92m1. You can only move the top number of a 'ring' on a tower.\n\
\x1b[91m2. You can only place a lesser number on top of a greater number.\n";

    while (full_input.empty() || full_input[0] < '1' || full_input[0] > '9') {
        std::cout <<
"\x1b[92mPlease type the amount of rings you want in the tower.\n\
\x1b[91mThis amount should be from 1 to 9, inclusive.\x1b[0m\n";

        std::getline(std::cin, full_input);
    }

    int32_t num_of_rings = full_input[0] - '0';

    constexpr uint64_t board_max_size = 9UL * 3;

    std::vector<int32_t> board{};
    board.reserve(board_max_size);
    board = get_new_board(num_of_rings);

    std::vector<int32_t> finished_board{};
    finished_board.reserve(board_max_size);
    finished_board = get_finished_board(num_of_rings);

    std::cout <<
"\x1b[93mType 'r' to reset the board,\n\
\x1b[92mor 'r[digit]' for a different board configuration,\n\
\x1b[91m'q' to quit,\n\
\x1b[93mor 'moves' to see how many moves you have made.\x1b[0m\n";

    while (true) {
        std::cout << format_board(board, num_of_rings) << '\n';

        // full_input = std::to_string(one(gen));
        // full_input.push_back(two(gen));
        // std::cout << full_input << '\n';

        std::getline(std::cin, full_input);
        const instruction instr = size_to_instructions(full_input);

        std::cout << instr.to_print;
        switch (instr.to_do) {
            case CONTINUE:
                continue;
            case NEW_BOARD:
                number_of_moves = 0;
                board = get_new_board(num_of_rings);
                continue;
            case NEW_GAME:
                number_of_moves = 0;
                num_of_rings = full_input[1] - '0';
                board = get_new_board(num_of_rings);
                finished_board = get_finished_board(num_of_rings);
                continue;
            case PRINT_MOVES:
                std::cout << "\x1b[92mNumber of moves is " << number_of_moves << ".\x1b[0m\n";
                continue;
            case QUIT:
                return 0;
            case PROCEED:
                break;
        }

        const index_pair pair = find_valid_move(board, num_of_rings, full_input[0] - '0', +full_input[1]);

        if (pair.x == 0 && pair.y == 0){
            std::cout << "\x1b[93mInvalid move.\x1b[0m\n";
            continue;
        }
        std::swap(board[pair.x], board[pair.y]);
        ++number_of_moves;
        if (board != finished_board){
            continue;
        }
        std::cout << format_board(board, num_of_rings) << '\n';
        std::cout <<
"\x1b[92mYou win in " << number_of_moves << " moves!\n\
Type a single digit to generate a new board, 'a' for the same board, or anything else to quit.\x1b[0m\n";
        number_of_moves = 0;
        std::getline(std::cin, full_input);
        if (full_input.empty()){
            return 0;
        }
        if (full_input[0] == 'a' || full_input[0] == 'A'){
            board = get_new_board(num_of_rings);
            continue;
        }
        if (full_input[0] < '1' || full_input[0] > '9'){
            return 0;
        }
        num_of_rings = full_input[0] - '0';
        board = get_new_board(num_of_rings);
        finished_board = get_finished_board(num_of_rings);
    }
    return 0;
}
