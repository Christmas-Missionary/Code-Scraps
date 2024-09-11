#include "typedefs.hpp"
#include <__thread/thread.h>
#include <cstdio>
#include <iostream>
#include "term.hpp"
#include "characters.hpp"

static bool get_control_set(){
    std::cout << "Which control set do you want?\n"
              << "Set 1 is 'qweasdzxc' to move and 'uiojklm,.' to shoot "
              << "while Set 2 is 'qweasdzxc' to shoot and 'uiojklm,.' to move.\n"
              << "[1/2], no return is needed.\n";

    int input = getchar();
    while (input != '1' && input != '2'){
        std::cout << "That is not 1 or 2. Try again\n";
        input = getchar();
    }
    return (input == '2');
}

int main(){
    // std::ios::sync_with_stdio(false);
    const termios oldt = get_termios();
    set_termios(turn_off_flags(oldt));
    std::cout << "Insert beginning test here.\n";
    const bool is_using_other_set = get_control_set();


    Characters::Player player = {{{Characters::board_size.x / 2, Characters::board_size.y / 2}, 'P'}, 20};

    std::thread bullet_manager(Characters::manage_bullets, &(player.to_see));
    std::thread enemy_spawner(Characters::spawn_enemies, &(player.to_see));
    std::thread enemy_manager(Characters::manage_enemies, &player);

    std::cout << Characters::format_board(Characters::board_size, player.to_see) << '\n';
    while (true){
        const auto input = static_cast<schar>(getchar());
        if (input == '\x1b'){ // x1b is the escape and arrow keys
            break;
        }
        const auto [whom, x, y] = Characters::input_to_dir(input, is_using_other_set);
        const Characters::ucoord next = Characters::next_move(player.to_see.coordinate, {x, y}, Characters::board_size);
        if (whom == Characters::direction::PLAYER){
            player.to_see.coordinate = next;
        } else { // Characters::direction::BULLET
            Characters::new_bullet(next, 'B', {x, y});
        }
        std::cout << Characters::format_board(Characters::board_size, player.to_see) << '\n';
    }

    bullet_manager.detach();
    enemy_spawner.detach();
    enemy_manager.detach();
    set_termios(oldt);
    std::cout << '\n';
    return 0;
}
