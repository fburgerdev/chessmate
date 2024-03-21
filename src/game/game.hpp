#pragma once
#include "board.hpp"

namespace Chessmate {
    // Game
    class Game {
    public:
        // Constructor
        Game();
        Game(const string& fen, bool drawclaimed = false);
        // Board
        Board& getBoard();
        const Board& getBoard() const;
        // Move
        Board& doMove(Move move);
        Board& undoMove();
        // Draw
        bool isDraw() const;
        void claimDraw();
        // GameOver
        bool isGameOver() const;
    private:
        // Member
        Stack<Board> m_boardhistory;
        bool m_drawclaimed;
    };
}