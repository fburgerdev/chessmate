#pragma once
#include "common.hpp"

namespace Chessmate {
    // Player
    enum class Player {
        None = 0, White, Black
    };
    // Enemy
    Player getEnemy(Player player);
    // FromPlayer / ToPlayer
    string fromPlayer(Player player);
    Player toPlayer(const string& str);
    // PlayerValue
    template<typename T>
    struct PlayerValue {
    public:
        // Constructor
        PlayerValue(T whitevalue, T blackvalue)
            : m_whitevalue(whitevalue), m_blackvalue(blackvalue) {}
        // Operator[]
        const T& operator[](Player player) const {
            return (player == Player::White) ? m_whitevalue : m_blackvalue;
        }
    private:
        // Member
        T m_whitevalue;
        T m_blackvalue;
    };
}