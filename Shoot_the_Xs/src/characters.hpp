#pragma once
#include "typedefs.hpp"
// #include <__chrono/high_resolution_clock.h>

namespace Characters {
    // using time_point = std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<int64_t, std::ratio<1LL, 1000000000LL>>>;

    struct ucoord {
        uchar x; // length
        uchar y; // width
    };

    struct scoord {
        schar x;
        schar y;

        explicit operator bool() const;
    };

    // temporary struct for simplifying char input.
    struct direction {
        enum WHOM: char {PLAYER, BULLET} whom;
        schar x;
        schar y;
    };

    constexpr ucoord board_size = {94, 40};

    struct avatar {
        ucoord coordinate;
        char icon;
    };

    struct Player {
        avatar to_see;
        uchar health;
    };

    struct Bullet {
        avatar to_see;
        scoord dir;
    };

    struct Enemy {
        avatar to_see;
        uchar health;
        // time_point start;
    };

    [[nodiscard, gnu::const]]
    direction input_to_dir(char input, bool other_set);

    [[nodiscard, gnu::const]]
    ucoord next_move(ucoord coor, scoord diff, ucoord board);

    /* ! All impure funcs are below ! */

    [[noreturn]]
    void manage_enemies(Player * player_ptr);

    [[noreturn]]
    void spawn_enemies(const avatar * player_ptr);

    [[noreturn]]
    void manage_bullets(const avatar * player_ptr);

    [[nodiscard]]
    const char * format_board(Characters::ucoord board, avatar player);

    void new_bullet(ucoord coordinate, char icon, scoord dir);

    void new_enemy(ucoord coordinate, char icon);

}
