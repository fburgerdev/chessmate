#pragma once
#include "game/board.hpp"

namespace Chessmate {
    // MinimaxResult
    struct MinimaxResult {
        // constructor
        MinimaxResult(Move move, float eval);
        
        // compare
        bool operator<(const MinimaxResult& other) const;
        bool operator<=(const MinimaxResult& other) const;
        bool operator>(const MinimaxResult& other) const;
        bool operator>=(const MinimaxResult& other) const;

        // move / eval
        Move move;
        float eval;
    };
    
    // minimize / maximize
    MinimaxResult minimize(const Board& board, uint32 depth, float alpha, float beta);
    MinimaxResult maximize(const Board& board, uint32 depth, float alpha, float beta);
    // minimax
    MinimaxResult minimax(const Board& board, uint32 depth);
}