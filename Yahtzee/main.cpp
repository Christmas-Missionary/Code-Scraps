#include "typedefs.hpp"

#include <__algorithm/adjacent_find.h>
#include <__algorithm/count.h>
#include <__algorithm/find.h>
#include <__algorithm/min.h>
#include <__algorithm/ranges_search.h>
#include <__algorithm/sort.h>
#include <__filesystem/operations.h>
#include <__iterator/advance.h>
#include <__iterator/distance.h>
#include <__numeric/accumulate.h>
#include <__random/mersenne_twister_engine.h>
#include <__random/random_device.h>
#include <__random/uniform_int_distribution.h>

#include <array>
#include <cassert>
#include <ctime>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

namespace { // * static

constexpr uchar num_of_total_boxes = 16;
constexpr uchar last_total_index = num_of_total_boxes - 1;
constexpr uchar num_of_scored_boxes = 13;
constexpr uchar num_of_dice = 5;

constexpr char yahtzee_score = 50;
constexpr char large_straight_score = 40;
constexpr char small_straight_score = 30;
constexpr char full_house_score = 25;

enum BOARD_LOWER_SECTION: uchar {
    THREE_OF_A_KIND = 6, FOUR_OF_A_KIND, FULL_HOUSE,
    SMALL_STRAIGHT, LARGE_STRAIGHT, CHANCE, YAHTZEE
};

constexpr std::array<const char *, num_of_total_boxes> total_boxes {
    "Ones", "Twos", "Threes", "Fours",
    "Fives", "Sixes", "Sum", "Bonus",
    "Three of a Kind", "Four of a Kind", "Full House", "Small Straight",
    "Large Straight", "Chance", "Yahtzee", "Total",
};

// structs are not needed, just typedefs
using board = std::array<char, num_of_scored_boxes>;
using dice_set = std::array<char, num_of_dice>;

constexpr board new_board = {-1, -1, -1, -1,
    -1, -1, -1, -1,
    -1, -1, -1, -1, -1};

struct bool_set {
    uchar set {};

    [[nodiscard]] constexpr bool operator[](const size_t index) const {
        // assertion is used for a dice set of 5
        assert(index >= 0 && index <= 4 && "Out of bounds for bool_set");
        return (set & (1 << index)) != 0;
    }

    // when someone presses a number between 1 to 5, the state to save or not save flips.
    void flip(const size_t index){
        // assertion is used for a dice set of 5
        assert(index >= 0 && index <= 4 && "Out of bounds for bool_set");
        set ^= (1 << index);
    }

    void change_set(std::string const& input){
        for (const char chr: input.substr(0, std::min(input.size(), 5UL))){
            if (chr >= '1' && chr <= '5'){
                set ^= (1 << (chr - '1'));
            }
        }
    }

};

struct sum_score {
    int16_t upper;
    int16_t total;

    [[nodiscard]] constexpr sum_score() = default;

    [[nodiscard]] constexpr sum_score(const char * const first, const char * const last){
        if (first == last){
            upper = 0;
            total = 0;
            return;
        }
        // ignores -1
        // the lambda itself is constexpr, not the return value,
        // as there are no variable expressions to capture.
        constexpr auto func = [](const int32_t sum, const int16_t val){
            return (val == -1) ? sum : sum + val;
        };
        // upper is sum from indices 0 to 5
        // total is sum from all indices given iterators (first is included, not last)
        const char * middle_itr = first;
        std::advance(middle_itr, 6);
        upper = std::accumulate(first, middle_itr, static_cast<int16_t>(0), func);
        total = std::accumulate(middle_itr, last, upper, func);
    }

    [[nodiscard]] constexpr int16_t get_bonus() const {
        constexpr int16_t sum_for_bonus = 63;
        constexpr int16_t bonus_score = 35;
        return (upper >= sum_for_bonus) ? bonus_score : 0;
    }

    // This is here to prevent reconstruction of sum_score everytime the game is printed.
    void add(const size_t index, const char value){
        if (index < THREE_OF_A_KIND){
            upper += value;
        }
        total += value;
    }
};

// This could have been just a function, but it contains state.
class DieGenerator {
    std::mt19937 gen;
    std::uniform_int_distribution<int8_t> dist;

public:
    [[nodiscard]] DieGenerator(){
        std::random_device rad;
        gen.seed(rad());
        dist.param(std::uniform_int_distribution<int8_t>::param_type(1, 6));
    }

    [[nodiscard]] dice_set operator()(dice_set dice, bool_set const& bools){
        for (size_t i = 0; i < num_of_dice; i++){
            if (!bools[i]){
                dice.at(i) = dist(gen);
            }
        }
        return dice;
    }
};

// just a return type for uncompress_game
// structured binding in main made this painless to unload.
struct Game {
    board board;
    dice_set dice;
    bool_set to_save;
    char rolls_left;
};

[[nodiscard]] std::string format_game(board const& scoreboard, board const& possible_boxes, sum_score const& sums, dice_set const& dice, bool_set const& to_save, const char rolls_left){
    std::stringstream res;

    for (size_t i = 0; i < 6; i++){
        res << total_boxes.at(i) << ": ";
        if (scoreboard.at(i) != -1){
            res << +scoreboard.at(i) << '\n';
            continue;
        }
        res << "X -> " << +possible_boxes.at(i) << "\n";
    }
    // get the sums from sum_score
    res << total_boxes.at(6) << ": " << sums.upper << '\n'
        << total_boxes.at(7) << ": " << sums.get_bonus() << '\n';
    for (size_t i = 8; i < last_total_index; i++){
        res << total_boxes.at(i) << ": ";
        if (scoreboard.at(i - 2) != -1){
            res << +scoreboard.at(i - 2) << '\n';
            continue;
        }
                    // char gets promoted to int to be shown as an int, not a char.
        res << "X -> " << +possible_boxes.at(i - 2) << "\n";
    }
    res << total_boxes.at(last_total_index) << ": " << sums.total << '\n';

    res << "Dice: ";
    for (size_t i = 0; i < num_of_dice; i++){
        res << (to_save[i] ? "\x1b[38;5;21m" : "\x1b[93m") << +dice.at(i) << ' ';
    }
    res << "\x1b[0m\nrolls left: " << +rolls_left << '\n';
    return res.str();
}

// static_cast<char> is too many letters
[[nodiscard]] constexpr inline char tochr(const int64_t num){
    return static_cast<char>(num);
}

// returns a board of possible options, needed for format_game
[[nodiscard]] constexpr board analyze(board const& scoreboard, dice_set const& dice){
    board res {};
    for (size_t i = 0; i < THREE_OF_A_KIND; i++){
        if (scoreboard.at(i) == -1){
            res.at(i) = tochr(std::count(dice.begin(), dice.end(), i + 1) * static_cast<int64_t>(i + 1));
        }
    }

    const char sum = std::accumulate(dice.begin(), dice.end(), '\0');

    res.at(CHANCE) = sum;
    if (std::count(dice.begin(), dice.end(), dice.at(0)) == 5){
        res.at(YAHTZEE) = yahtzee_score;
    }

    dice_set sorted = dice;
    std::sort(sorted.begin(), sorted.end());

    if ((sorted.at(0) == sorted.at(1) && sorted.at(0) == sorted.at(2)) ||
        (sorted.at(1) == sorted.at(2) && sorted.at(1) == sorted.at(3)) ||
        (sorted.at(2) == sorted.at(3) && sorted.at(2) == sorted.at(4))){
            res.at(THREE_OF_A_KIND) = sum; // Three of a Kind
    }

    if ((sorted.at(0) == sorted.at(1) && sorted.at(0) == sorted.at(2) && sorted.at(0) == sorted.at(3)) ||
        (sorted.at(1) == sorted.at(2) && sorted.at(1) == sorted.at(3) && sorted.at(1) == sorted.at(4))){
            res.at(FOUR_OF_A_KIND) = sum;
    }


    if (res.at(YAHTZEE) != yahtzee_score &&
        sorted.at(0) == sorted.at(1) && sorted.at(3) == sorted.at(4) &&
       (sorted.at(2) == sorted.at(1) || sorted.at(2) == sorted.at(3))){
            res.at(FULL_HOUSE) = full_house_score;
    }

    // makes the list to have each element that is from 1 to 6 be unique, makes it easier to find a straight.
    {
    char * const ptr = std::adjacent_find(sorted.begin(), sorted.end());
    if (ptr != sorted.end()){
        *ptr = 0;
    }
    std::sort(sorted.begin(), sorted.end());
    }

    for (std::array<char, 4> const& arr: {
        std::array<char, 4>{1, 2, 3, 4},
        {2, 3, 4, 5},
        {3, 4, 5, 6}})
        {
        auto subrange = std::ranges::search(sorted.begin(), sorted.end(), arr.begin(), arr.end());
        if (subrange.begin() != subrange.end()){
            res.at(SMALL_STRAIGHT) = small_straight_score;
            break;
        }
    }

    if (sorted == dice_set{1, 2, 3, 4, 5} ||
        sorted == dice_set{2, 3, 4, 5, 6}){
            res.at(LARGE_STRAIGHT) = large_straight_score;
        }

    return res;
}

// Limited to MacOS, and limited to executing under "Users", not /usr/local/bin.
[[nodiscard]] std::string find_user_path(std::string const& path){
    const std::string users_dir = "/Users/";
    const size_t pos = path.find(users_dir);
    if (pos == std::string::npos){
        return "";
    }

    const auto iter = path.begin()
        + static_cast<std::iterator_traits<const char *>::difference_type>(pos + users_dir.size());

    const std::string username = std::string(iter, std::find(iter, path.end(), '/'));
    return users_dir + username;
}

[[nodiscard]] constexpr size_t input_to_board_index(std::string const& input){
    constexpr std::array<const char *, num_of_scored_boxes> board_boxes {
    "Ones", "Twos", "Threes", "Fours", "Fives",
    "Sixes", "Three of a Kind", "Four of a Kind", "Full House",
    "Small Straight", "Large Straight", "Chance", "Yahtzee",
    };
    // ptr is a constant pointer to a constant "const char *" or C-String.
    const char * const * const ptr = std::find(board_boxes.begin(), board_boxes.end(), input);
    return (ptr != board_boxes.end()) ? static_cast<size_t>(std::distance(board_boxes.begin(), ptr)) : -1UL;
}


[[nodiscard]] std::string format_finished_game(board const& scoreboard, sum_score const& sums){
    std::stringstream res;
    for (size_t i = 0; i < THREE_OF_A_KIND; i++){
        res << total_boxes.at(i) << ": " << +scoreboard.at(i) << '\n';
    }
    res << total_boxes.at(6) << ": " << sums.upper << '\n' << total_boxes.at(7) << ": " << sums.get_bonus() << '\n';
    for (size_t i = 8; i < last_total_index; i++){
        res << total_boxes.at(i) << ": " << +scoreboard.at(i - 2) << '\n';
    }
    res << total_boxes.at(last_total_index) << ": " << sums.total << '\n';

    return res.str();
}

[[nodiscard]] std::string add_time_to_str(std::string str, time_t const& now){
    str += std::asctime(std::gmtime(&now));
    str.pop_back();
    str += " UTC\n";
    return str;
}

// I recommend switch statements as their own functions, or you will be doing "break;" for each line.
[[nodiscard]] constexpr char box_to_score(const BOARD_LOWER_SECTION box){
    switch (box) {
        case FULL_HOUSE: return full_house_score;
        case SMALL_STRAIGHT: return small_straight_score;
        case LARGE_STRAIGHT: return large_straight_score;
        case YAHTZEE: return yahtzee_score;
        default: return tochr(-1);
    }
};

[[nodiscard]] std::string compress_game(board const& board, dice_set const& dice, bool_set const& to_save, const char rolls_left){
    // if this is a starting/blank game, don't compress.
    if (dice == dice_set{} && to_save.set == 0 && rolls_left == 3 && board == new_board){
        return "";
    }

    constexpr auto compress_upper = [](const char value, const char index){
        return (value == -1) ? 7 : (value / (index + 1));
    };

    constexpr auto compress_lower = [](const char value, const BOARD_LOWER_SECTION box){
        return (value == -1) ? '\2'
             : (value == box_to_score(box)) ? '\1'
             : '\0';
    };

    constexpr auto to_unknown_sum = [](const char value){
        constexpr char unknown_value = 0b1'1111;
        return (value == -1) ? unknown_value : value;
    };

    std::string res(8, 0);

    res.at(0) = tochr((dice.at(0) << 5) + to_save.set);
    res.at(1) = tochr((dice.at(1) << 5) + (rolls_left << 3) + compress_upper(board.at(0), 0));
    res.at(2) = tochr((dice.at(2) << 5) + (compress_lower(board.at(FULL_HOUSE), FULL_HOUSE) << 3) + compress_upper(board.at(1), 1));
    res.at(3) = tochr((dice.at(3) << 5) + (compress_lower(board.at(SMALL_STRAIGHT), SMALL_STRAIGHT) << 3) + compress_upper(board.at(2), 2));
    res.at(4) = tochr((dice.at(4) << 5) + (compress_lower(board.at(LARGE_STRAIGHT), LARGE_STRAIGHT) << 3) + compress_upper(board.at(3), 3));
    res.at(5) = tochr((compress_upper(board.at(4), 4) << 5) + to_unknown_sum(board.at(THREE_OF_A_KIND)));
    res.at(6) = tochr((compress_upper(board.at(5), 5) << 5) + to_unknown_sum(board.at(FOUR_OF_A_KIND)));
    res.at(7) = tochr((compress_lower(board.at(YAHTZEE), YAHTZEE) << 6) + (1 << 5) + to_unknown_sum(board.at(CHANCE)));
    return res;
}

[[nodiscard]] constexpr Game uncompress_game(std::string const& save){
    // size of the str save must be at least 8 bytes.
    if (save.size() < 8){
        return {new_board, {}, {}, 3};
    }

    constexpr uchar last_three = 0b1110'0000;
    constexpr uchar middle_two = 0b0001'1000;
    constexpr uchar first_three = 0b0000'0111;
    constexpr uchar first_five = 0b0001'1111;
    constexpr uchar last_two = 0b1100'0000;

    constexpr auto uncompress_upper = [](const int count, const char index){
        return tochr((count == 7) ? -1 : count * (index + 1));
    };

    constexpr auto uncompress_lower = [](const int value, const BOARD_LOWER_SECTION box){
        return (value == '\2') ? tochr(-1)
             : (value == '\1') ? box_to_score(box)
             : '\0';
    };

    constexpr auto from_unknown_sum = [](const int value){
        return tochr((value == first_five) ? -1 : value);
    };

    Game res {};

    res.to_save.set = save.at(0) & first_five;
    res.rolls_left = tochr((save.at(1) & middle_two) >> 3);

    for (uchar i = 0; i < 5; i++){
        res.dice.at(i) = tochr((save.at(i) & last_three) >> 5);
    }

    for (uchar i = 0; i < 4; i++){
        res.board.at(i) = uncompress_upper(save.at(i + 1) & first_three, static_cast<char>(i));
    }

    res.board.at(4) = uncompress_upper((save.at(5) & last_three) >> 5, 4);
    res.board.at(5) = uncompress_upper((save.at(6) & last_three) >> 5, 5);

    res.board.at(THREE_OF_A_KIND) = from_unknown_sum(save.at(5) & first_five);
    res.board.at(FOUR_OF_A_KIND) = from_unknown_sum(save.at(6) & first_five);
    res.board.at(CHANCE) = from_unknown_sum(save.at(7) & first_five);

    for (BOARD_LOWER_SECTION index: {FULL_HOUSE, SMALL_STRAIGHT, LARGE_STRAIGHT}){
        res.board.at(index) = uncompress_lower((save.at(index - 6) & middle_two) >> 3, index);
    }

    res.board.at(YAHTZEE) = uncompress_lower((save.at(7) & last_two) >> 6, YAHTZEE);

    return res;
}

// !! !!
// !! FUNCTIONS AFTER THIS POINT HAVE OUTSIDE "SIDE EFFECTS". !!
// !! !!

// !! Side effects: std::ofstream !!
void print_to_file(std::string const& to_print, std::string const& file_path){
    std::ofstream file(file_path);
    file << to_print;
    file.flush();
    file.close();
}

// !! Side effects: std::ifstream !!
[[nodiscard]] std::string load_from_file(std::string const& file_path){
    std::string res {};
    std::ifstream file(file_path);
    if (file.is_open()){
        file >> res;
        file.close();
    }
    return res;
}

}; // * end static

// !! Side effects: std::cout, std::cin, std::time, std::filesystem::create_directories, !!
// !! All of the above !!
int main(const int _argc, const char **argv){
    (void)_argc; // silences unused variable warning.

    constexpr uchar all_dice_saved = 0b0001'1111;
    constexpr const char * save_folder = "/.Yahtzee_Terminal_Game/";
    constexpr const char * save_file = "Save.yhtz";
    const std::string user_path = find_user_path(*argv);

    // !! State in bytes per variable (13 + 5 + 1 + 1 + 4 + 2512 + x≥24) !!
    auto [scoreboard, dice, to_save, rolls_left]
        = uncompress_game(load_from_file(user_path + save_folder + save_file));

    sum_score sums(scoreboard.begin(), scoreboard.end());
    DieGenerator gen {};
    std::string full_input;
    // !! State !!

    if (dice != dice_set{} || to_save.set != 0 || rolls_left != 3 || scoreboard != new_board){
        std::cout << format_game(scoreboard, analyze(scoreboard, dice), sums, dice, to_save, rolls_left);
    } else {
        std::cout << "Hello, this is Yahtzee!\n"
                  << "I didn't have the interest to really beta test this, so I recommend not quitting this if you plan on finishing a game.\n"
                  << "Please press 'r' to roll the dice.\n";
    }

    while (true){ // game loop

        std::getline(std::cin, full_input);
        if (full_input.empty()){
            std::cout << "Please type something.\n";
            continue;
        }

        const char command = full_input.at(0);

        if (command == 'q'){
            std::filesystem::create_directories(user_path + save_folder);
            const std::string save_str = compress_game(scoreboard, dice, to_save, rolls_left);
            print_to_file(save_str, user_path + save_folder + save_file);
            if (!save_str.empty()){
                std::cout << "Game saved.\n";
            }
            return 0;
        }
        if (command == 'n'){
            scoreboard = new_board;
            dice = dice_set{};
            to_save = bool_set{};
            rolls_left = 3;
            sums = sum_score{};
            gen = DieGenerator{};

            std::cout << "New game has begun.\nType 'q' and then reexecute to reload last game.\n";
            continue;
        }

        if (command == 'r'){
            if (to_save.set == all_dice_saved){
                std::cout << "There is no dice to roll.\n";
                continue;
            }
            if (rolls_left == 0){
                std::cout << "No rolls left, please fill out a box.\n";
                continue;
            }
            dice = gen(dice, to_save);
            --rolls_left;
            std::cout <<
                format_game(scoreboard, analyze(scoreboard, dice), sums, dice, to_save, rolls_left)
                << '\n';
            continue;
        }
        if (rolls_left == 3){
            std::cout << "Please roll the dice.\n";
            continue;
        }
        const board possible_boxes = analyze(scoreboard, dice);
        if (command > '0' && command < '6'){
            to_save.change_set(full_input);
            std::cout <<
                format_game(scoreboard, possible_boxes, sums, dice, to_save, rolls_left)
                << '\n';
            continue;
        }

        const size_t index = input_to_board_index(full_input);

        if (index == -1UL){
            std::cout <<
                ((rolls_left == 0)
                ? "Type the name of a box.\n"
                : "Please type something meaningful.\n");
            continue;
        }

        if (scoreboard.at(index) != -1){
            std::cout << "Box has already been taken.\n";
            continue;
        }
        scoreboard.at(index) = possible_boxes.at(index);
        sums.add(index, scoreboard.at(index));

        if (std::find(scoreboard.begin(), scoreboard.end(), -1) == scoreboard.end()){

            const std::string finished_game =
                add_time_to_str(
                format_finished_game(scoreboard, sums), std::time(nullptr));
            std::cout << finished_game;
            std::cout << "There are now no more boxes to fill out. The game has ended.\nPlay Again? Export Game to file? [y/e/n or anything else]\n";
            std::getline(std::cin, full_input);

            if (!full_input.empty() && (full_input.at(0) == 'E' || full_input.at(0) == 'e')){
                std::cout << "Type the name of your file. No extension is needed.\n";
                std::getline(std::cin, full_input);
                print_to_file(finished_game,
                std::string(user_path).append("/Desktop/").append(full_input).append(".txt"));
                std::cout << "Finished game has been exported to desktop.\nPlay Again? [y/n or anything else]\n";
                std::getline(std::cin, full_input);
            }
            if (full_input.empty() || (full_input.at(0) != 'Y' && full_input.at(0) != 'y')){
                print_to_file("", user_path + save_folder + save_file);
                return 0;
            }
            scoreboard = new_board;
            sums = {};
        }

        to_save = {};
        dice = gen(dice, to_save);
        rolls_left = 2;
        std::cout <<
            format_game(scoreboard, analyze(scoreboard, dice), sums, dice, to_save, rolls_left)
            << "The dice have been rolled for you.\n";

    } // end game loop
    return 0;
}
