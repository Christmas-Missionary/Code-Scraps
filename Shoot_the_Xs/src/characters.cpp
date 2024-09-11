#include "characters.hpp"
#include <__iterator/next.h>
#include <__random/uniform_int_distribution.h>
#include <__random/mersenne_twister_engine.h>
#include <__random/random_device.h>
#include <__thread/this_thread.h>
#include <array>
#include <iostream>
#include <list>
#include <string>

namespace {

template<Characters::ucoord size>
[[nodiscard, gnu::const]] consteval
uint16_t calc_size(){
    static_assert(size.x >= 5 && size.y >= 5, "Any side Lengths below 5 do not make 3 rings.");
    Characters::ucoord tracer = size;
    if (size.x == 5 && size.y == 5){
        return 5 * 5;
    }
    uint16_t res {};
    for (uchar i = 0; i < 3; i++){
        res += (tracer.x * 2) + (tracer.y * 2) - 4;
        tracer.x -= 2;
        tracer.y -= 2;
    }
    return res;
}

// The template is to avoid using a vector.
template<Characters::ucoord board_s>
[[nodiscard, gnu::const]] consteval
std::array<uint16_t, calc_size<board_s>()> three_rings(){
    constexpr uint16_t array_size = calc_size<board_s>();
    constexpr auto overflow = static_cast<uint16_t>(-1);
    std::array<uint16_t, array_size> res {};
    uint16_t minx {};
    uint16_t maxx = board_s.x;
    uint16_t miny {};
    uint16_t maxy = board_s.y;
    uint16_t tracer_x {};
    uint16_t tracer_y {};
    for (uint16_t i = 0;;){
        for (tracer_x = minx; tracer_x < maxx; tracer_x++){
            res[i] = tracer_x + (tracer_y * board_s.x);
            if (++i >= array_size){
                return res;
            }
        }
        --tracer_x;
        ++miny;
        for (tracer_y = miny; tracer_y < maxy; tracer_y++){
            res[i] = tracer_x + (tracer_y * board_s.x);
            if (++i >= array_size){
                return res;
            }
        }
        --tracer_y;
        --maxx;
        for (tracer_x = maxx - 1; tracer_x >= minx && tracer_x != overflow; tracer_x--){
            res[i] = tracer_x + (tracer_y * board_s.x);
            if (++i >= array_size){
                return res;
            }
        }
        ++tracer_x;
        --maxy;
        for (tracer_y = maxy - 1; tracer_y >= miny; tracer_y--){
            res[i] = tracer_x + (tracer_y * board_s.x);
            if (++i >= array_size){
                return res;
            }
        }
        ++tracer_y;
        ++minx;
    }
    return res;
}

[[nodiscard, gnu::const]] constexpr
Characters::scoord get_offset_dir(const Characters::ucoord target, const Characters::ucoord you){
    using const_scoor = const Characters::scoord;
    const_scoor coordinate = {static_cast<schar>(target.x - you.x), static_cast<schar>(target.y - you.y)};

    enum TWO_SIGNS: char {POSPOS, POSNEG, NEGPOS, NEGNEG};
    constexpr auto two_signs = [](const_scoor cor){
        return (cor.x >= 0)
             ? (cor.y >= 0) ? POSPOS : POSNEG
             : (cor.y >= 0) ? NEGPOS : NEGNEG;
    };
    const double abs_slope = std::abs((coordinate.y * 1.0) / coordinate.x);
    const auto double_ternary = [abs_slope](const_scoor one, const_scoor two, const_scoor three){
        return (abs_slope < (0.5)) ? one : (abs_slope < 2.0) ? two : three;
    };
    switch (two_signs(coordinate)){
        case POSPOS:
            return double_ternary({1, 0}, {1, 1}, {0, 1});
        case POSNEG:
            return double_ternary({1, 0}, {1, -1}, {0, -1});
        case NEGPOS:
            return double_ternary({-1, 0}, {-1, 1}, {0, 1});
        case NEGNEG:
            return double_ternary({-1, 0}, {-1, -1}, {0, -1});
    }
}

// use std::cout << format(board_indices, board_size) << '\n'; to test if three_rings() worked.
// template<size_t num_of_indices>
// [[nodiscard, gnu::pure]]
// std::string format(std::array<uint16_t, num_of_indices> const& arr, const Characters::ucoord board_size){
//     const auto area = static_cast<size_t>(board_size.x * board_size.y);

//     std::string res(area, ' ');
//     for (uint16_t const& index: arr){
//         res[index] = 'X';
//     }
//     for (size_t i = area; i > 0; i -= board_size.x){
//         res.insert(i, "\n");
//     }
//     return res;
// }

[[nodiscard, gnu::const]] constexpr
bool operator==(const Characters::ucoord ori, const Characters::ucoord other) {
    return ori.x == other.x && ori.y == other.y;
}

class Generator {

    std::mt19937 rng;
    std::uniform_int_distribution<uint16_t> dist;

public:

    [[nodiscard]]
    Generator(const uint16_t min, const uint16_t max){
        std::random_device _rd {};
        rng.seed(_rd());
        dist.param(std::uniform_int_distribution<uint16_t>::param_type{min, max});
    }

    [[nodiscard]]
    uint16_t operator()(){
        return dist(rng);
    }

};

}

Characters::scoord::operator bool() const {
    return x != 0 || y != 0;
}

Characters::direction Characters::input_to_dir(const char input, const bool other_set){
    const auto top = (other_set) ? direction::BULLET : direction::PLAYER;
    const auto bottom = (other_set) ? direction::PLAYER : direction::BULLET;
    switch (input) {
        case 'q': return {top, -1, -1};
        case 'w': return {top, 0, -1};
        case 'e': return {top, 1, -1};
        case 'a': return {top, -1, 0};
        case 'd': return {top, 1, 0};
        case 'z': return {top, -1, 1};
        case 'x':
        case 's': return {top, 0, 1};
        case 'c': return {top, 1, 1};

        case 'u': return {bottom, -1, -1};
        case 'i': return {bottom, 0, -1};
        case 'o': return {bottom, 1, -1};
        case 'j': return {bottom, -1, 0};
        case 'l': return {bottom, 1, 0};
        case 'm': return {bottom, -1, 1};
        case ',':
        case 'k': return {bottom, 0, 1};
        case '.': return {bottom, 1, 1};
        default: return {};
    }
}

// board length or width must not be 255, where uchar underflows to.
Characters::ucoord Characters::next_move(const ucoord coor, const scoord diff, const ucoord board) {
    const ucoord new_coord = {static_cast<uchar>(coor.x + diff.x), static_cast<uchar>(coor.y + diff.y)};
    return (new_coord.x < board.x && new_coord.y < board.y) ? new_coord : coor;
}

namespace {
    std::list<Characters::Bullet> all_bullets {};
    std::list<Characters::Enemy> all_enemies {};
}

void Characters::new_bullet(const ucoord coordinate, const char icon, const scoord dir){
    for (auto enemy_itr = all_enemies.begin(); enemy_itr != all_enemies.end(); enemy_itr = std::next(enemy_itr)){
        if (enemy_itr->to_see.coordinate == coordinate){
            if ((--enemy_itr->health) == 0){
                all_enemies.erase(enemy_itr);
            }
            return;
        }

    }
    all_bullets.push_back({{coordinate, icon}, dir});
}

void Characters::new_enemy(const ucoord coordinate, const char icon){
    all_enemies.push_back({{coordinate, icon}, 5});
}

// * add escape color text in here
const char * Characters::format_board(const ucoord board, const avatar player){
    const size_t board_length = board.x;
    const size_t board_area = board_length * board.y;

    std::string res;
    res.reserve(board_area + (board_length * 6));
    res = std::string(board_area, ' ');

    // func to get avatars of bullets and enemies?.
    for (auto const& bullet: all_bullets){
        res.at(bullet.to_see.coordinate.y * board_length + bullet.to_see.coordinate.x) = bullet.to_see.icon;
    }
    for (auto const& enemy: all_enemies){;
        res.at(enemy.to_see.coordinate.y * board_length + enemy.to_see.coordinate.x) = enemy.to_see.icon;
    }
    res.at(player.coordinate.y * board_length + player.coordinate.x) = player.icon;

    res.insert(board_area, "|\n");
    for (size_t i = board_area - board_length; i > 0; i -= board_length){
        res.insert(i, "|\n|");
    }

    const std::string first_and_last = '#' + std::string(board_length, '-') + "#\n";
    res = first_and_last + "|" + res + first_and_last;
    const char * to_return = res.c_str();
    return to_return;
}

[[noreturn]]
void Characters::spawn_enemies(const avatar * const player_ptr){
    constexpr auto board_indices = three_rings<board_size>();
    constexpr auto index_to_ucoord = [](const size_t index, const Characters::ucoord size_of_board){
        return Characters::ucoord{
            static_cast<uchar>(index % size_of_board.x),
            static_cast<uchar>(index / size_of_board.x)
            };
    };
    Generator rng {0, board_indices.size() - 1};
    while (true){
        new_enemy(index_to_ucoord(board_indices.at(rng()), board_size), 'X');
        std::cout << Characters::format_board(Characters::board_size, *player_ptr) << std::endl;
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
        // sleep value will be changable.
    }
}

[[noreturn]]
void Characters::manage_enemies(Player * const player_ptr){

    while (true){
        for (auto itr = all_enemies.begin(); itr != all_enemies.end(); itr = std::next(itr)){
            if (itr->to_see.coordinate != player_ptr->to_see.coordinate){
                const ucoord dir = next_move(
                    itr->to_see.coordinate,
                    get_offset_dir(player_ptr->to_see.coordinate, itr->to_see.coordinate),
                    board_size);
                if (dir == player_ptr->to_see.coordinate){
                    --player_ptr->health;
                    if (player_ptr->health == 0){
                        // delay threads, everything is removed,
                        using namespace std::chrono_literals;
                        std::this_thread::sleep_for(3s);
                        goto getout;

                    }
                    continue;
                }
                itr->to_see.coordinate = dir;
            }
        }
        if (!all_enemies.empty()){
            std::cout << Characters::format_board(board_size, player_ptr->to_see) << '\n';
        }
        getout:
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1s);
        // sleep value will be changable.
    }
}

[[noreturn]]
void Characters::manage_bullets(const avatar * const player_ptr){
    while (true){
        for (auto itr = all_bullets.begin(); itr != all_bullets.end();){
            const Characters::ucoord old_coord = itr->to_see.coordinate;
            itr->to_see.coordinate = next_move(old_coord, itr->dir, Characters::board_size);
            for (auto enemy_itr = all_enemies.begin(); enemy_itr != all_enemies.end(); enemy_itr = std::next(enemy_itr)){
                if (enemy_itr->to_see.coordinate == itr->to_see.coordinate){
                    if ((--enemy_itr->health) == 0){
                        all_enemies.erase(enemy_itr);
                    }
                    const auto next_itr = std::next(itr);
                    all_bullets.erase(itr);
                    itr = next_itr;
                    goto next_bullet;
                }

            }
            if (itr->to_see.coordinate == old_coord){
                const auto next_itr = std::next(itr);
                all_bullets.erase(itr);
                itr = next_itr;
            }
            itr = std::next(itr);
            next_bullet: ; // when bullet no longer exists after being hit
        }
        if (!all_bullets.empty()){
            std::cout << Characters::format_board(Characters::board_size, *player_ptr) << std::endl;
        }
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(25ms);
        // sleep value will NOT be changable.
    }
}
