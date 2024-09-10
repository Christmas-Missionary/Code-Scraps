#include <array>
#include <cstdint>
#include <string>
#include <iostream>
#include <utility>
#include <random>

const std::string largest_board = "123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ ";
const u_int64_t board_size = largest_board.size();

using u_char = unsigned char;

std::string format_board_as_str(std::string board, u_char row_length){
    if (board.size() % row_length != 0){
        return "";
    }
    const u_int64_t num_of_rows = board.size() / row_length;
    const u_char pos_for_first_newline = row_length;
    board.insert(row_length, "\n");
    ++row_length;
    for (u_int64_t i = 1; i < num_of_rows; i++){
        board.insert(static_cast<u_int64_t>(i * row_length) + pos_for_first_newline, "\n");
    }
    return board;
}

std::string shuffle_board(std::string to_shuffle, const u_char row_length){
    if (row_length <= 0){
        return to_shuffle;
    }

    std::random_device rand_d;
    std::mt19937 rng {rand_d()};
    std::uniform_int_distribution<u_int32_t> dist(0, 3);
    u_int32_t rand_num = 0;
    u_int64_t space_pos = to_shuffle.find(' ');
    u_int64_t new_pos = 0;
    const u_char pos_at_length_end = row_length - 1;
    const u_int64_t size = to_shuffle.size();
    const std::array<int16_t, 4> increments = {row_length, 1, static_cast<int16_t>(-row_length), -1};
    constexpr int32_t loop_amount = 200'000;
    for (int32_t i = 0; i < loop_amount; i++){
        rand_num = dist(rng);
        new_pos = space_pos + increments[rand_num];

        if  (new_pos >= size ||
            (new_pos % row_length == 0 && rand_num == 1) ||
            (new_pos % row_length == pos_at_length_end && rand_num == 3)){
            continue;
        }
        std::swap(to_shuffle[space_pos], to_shuffle[new_pos]);
        space_pos = new_pos;
    }
    return to_shuffle;
}

std::string get_board(const u_int32_t size){
    return (size < board_size) ? largest_board.substr(0, size - 1) + " " : largest_board;
}



int main(){
    std::string board;
    std::string final_board;
    u_char row_length = 0;
    u_int64_t space_pos = 0;
    u_int64_t new_pos = 0;
    u_int64_t num_of_moves = 0;

    std::string full_input;
    std::cout << "Input the board size you want from 2x2 to 6x6. It can be of varying length and width.\n";
    while (true){
        std::getline(std::cin, full_input);
        if (full_input.size() < 3){
            std::cout << "Too small of an input.\n";
            continue;
        }
        const std::string filtered_input = full_input.substr(0, 3);
        if (filtered_input[0] >= '2' && filtered_input[0] <= '6' &&
            filtered_input[1] == 'x' &&
            filtered_input[2] >= '2' && filtered_input[2] <= '6'){

            row_length = static_cast<u_char>(filtered_input[0] - '0');
            final_board = get_board(static_cast<u_int32_t>((filtered_input[0] - '0') * (filtered_input[2] - '0')));
            board = shuffle_board(final_board, row_length);
            space_pos = board.find(' ');
            break;
        }
        std::cout << "Invalid board size\n";
    }
    while (true) {
        std::cout << format_board_as_str(board, row_length);

        std::getline(std::cin, full_input);
        if (full_input.empty()){
            continue;
        }
        const char filtered = full_input[0];

        switch (filtered) {
        case 'w': new_pos = space_pos + row_length; break;
        case 'a': new_pos = space_pos + 1; break;
        case 's': new_pos = space_pos - row_length; break;
        case 'd': new_pos = space_pos - 1; break;
        case 'q': return 0;
        default: continue;
        }
        if  (new_pos >= board.size() ||
            (new_pos % row_length == 0 && filtered == 'a') ||
            (new_pos % row_length == (row_length - 1) && filtered == 'd')){
            std::cout << "Not a valid direction.\n";
            continue;
        }
        std::swap(board[space_pos], board[new_pos]);
        space_pos = new_pos;
        ++num_of_moves;

        if (board != final_board){
            continue;
        }

        std::cout << format_board_as_str(board, row_length);

        std::cout << "Your total number of moves is " << num_of_moves
                  << ". Different board size or play again? [p/a/NxN]\n";
        new_pos = 0;
        num_of_moves = 0;
        std::getline(std::cin, full_input);

        const std::string filtered_input = full_input.substr(0, 3);
        if (filtered_input[0] == 'p' || filtered_input[0] == 'a'){
            board = shuffle_board(get_board(static_cast<u_int32_t>(board.size())), row_length);
            space_pos = board.find(' ');
            continue;
        }

        if (filtered_input[0] >= '2' && filtered_input[0] <= '6' &&
            filtered_input[1] == 'x' &&
            filtered_input[2] >= '2' && filtered_input[2] <= '6'){
            row_length = static_cast<u_char>(filtered_input[0] - '0');
            final_board = get_board(static_cast<u_int32_t>((filtered_input[0] - '0') * (filtered_input[2] - '0')));
            board = shuffle_board(final_board, row_length);
            space_pos = board.find(' ');
            continue;
        }
        return 0;
    }

    return 0;
}
