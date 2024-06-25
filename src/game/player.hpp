#pragma once
#include "common.hpp"

namespace Chessmate {
    // Player
    enum class Player {
        None = 0, White, Black
    };
    // getEnemy
    Player getEnemy(Player player);
}