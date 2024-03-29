#pragma once
#include "game/board.hpp"

namespace Chessmate {
    // MinimaxResult
    struct MinimaxResult {
        // Constructor
        MinimaxResult(Move move, float eval);
        // Compare
        bool operator<(const MinimaxResult& other) const;
        bool operator<=(const MinimaxResult& other) const;
        bool operator>(const MinimaxResult& other) const;
        bool operator>=(const MinimaxResult& other) const;
        // Member
        Move move;
        float eval;
    };
    // Minimize / Maximize
    MinimaxResult minimize(const Board& board, uint32 depth, float alpha, float beta);
    MinimaxResult maximize(const Board& board, uint32 depth, float alpha, float beta);
    // Minimax
    MinimaxResult minimax(const Board& board, uint32 depth);
}