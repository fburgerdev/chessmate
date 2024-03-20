#include "game.hpp"

namespace Chessmate {
    // Game
    // Game :: Constructor
    Game::Game()
        : Game("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1") {}
    Game::Game(const string& fen, bool drawclaimed)
        : m_boardhistory(), m_drawclaimed(drawclaimed) {
        m_boardhistory.emplace(fen);
    }
    // Game :: Board
    Board& Game::getBoard() {
        return m_boardhistory.top();
    }
    const Board& Game::getBoard() const {
        return m_boardhistory.top();
    }
    // Game :: (DoMove / UndoMove)
    Board& Game::doMove(Move move) {
        return m_boardhistory.emplace(getBoard(), move);
    }
    Board& Game::undoMove() {
        m_boardhistory.pop();
        return m_boardhistory.top();
    }
    // Game :: Draw
    bool Game::isDraw() const {
        return m_drawclaimed || (!getBoard().inCheckmate() && getBoard().getLegalMoves().size() == 0);
    }
    void Game::claimDraw() {
        m_drawclaimed = true;
    }
    // Game :: IsGameOver
    bool Game::isGameOver() const {
        return getBoard().inCheckmate() || isDraw();
    }
}