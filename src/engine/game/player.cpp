#include "player.hpp"

namespace Chessmate {
    // getEnemy
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
}