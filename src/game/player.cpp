#include "player.hpp"

namespace Chessmate {
    // Enemy
    Player getEnemy(Player player) {
        switch (player) {
        case Player::White:
            return Player::Black;
        case Player::Black:
            return Player::White;
        default:
            return Player::None;
        }
    }
    // FromPlayer / ToPlayer
    string fromPlayer(Player player) {
        switch (player) {
        case Player::White:
            return "w";
        case Player::Black:
            return "b";
        case Player::None:
            return "none";
        default:
            return string();
        }
    }
    Player toPlayer(const string& str) {
        if (str == "w") {
            return Player::White;
        }
        else if (str == "b") {
            return Player::Black;
        }
        return Player::None;
    }
}