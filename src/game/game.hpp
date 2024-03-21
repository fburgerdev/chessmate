#pragma once
#include "board.hpp"

namespace Chessmate {
    // Game
    class Game {
    public:
        // Constructor
        Game();
        Game(const List<Move>& movelist);
        Game(const string& fen, bool drawclaimed = false);
        // Board
        Board& getBoard();
        const Board& getBoard() const;
        const List<Board>& getBoardList() const;
        // Move
        Board& doMove(const string& notation);
        Board& doMove(Move move);
        Board& undoMove();
        // Draw
        bool isDraw() const;
        void claimDraw();
        // GameOver
        bool isGameOver() const;
    private:
        // Member
        List<Board> m_boardhistory;
        bool m_drawclaimed;
    };
    // PGN
    Game fromPGN(const string& filepath);
}