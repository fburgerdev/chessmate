#include "game.hpp"
#include "string.hpp"
#include <fstream> // std::ifstream
#include <sstream> // std::stringstream
#include <iostream> // DEBUG

namespace Chessmate {
    // Game
    // Game :: Constructor
    Game::Game()
        : Game(List<Move>()) {}
    Game::Game(const List<Move>& movelist)
        : m_boardhistory(), m_drawclaimed(false) {
        m_boardhistory.emplace_back("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
        for (Move move : movelist) {
            doMove(move);
        }
    }
    Game::Game(const string& fen, bool drawclaimed)
        : m_boardhistory(), m_drawclaimed(drawclaimed) {
        m_boardhistory.emplace_back(fen);
    }
    // Game :: Board
    Board& Game::getBoard() {
        return m_boardhistory.back();
    }
    const Board& Game::getBoard() const {
        return m_boardhistory.back();
    }
    const List<Board>& Game::getBoardList() const {
        return m_boardhistory;
    }
    // Game :: Move
    Board& Game::doMove(const string& notation) {
        return m_boardhistory.emplace_back(getBoard(), getBoard().fromAlgebraicNotation(notation));
    }
    Board& Game::doMove(Move move) {
        return m_boardhistory.emplace_back(getBoard(), move);
    }
    Board& Game::undoMove() {
        m_boardhistory.pop_back();
        return m_boardhistory.back();
    }
    // Game :: Draw
    bool Game::isDraw() const {
        return m_drawclaimed || (!getBoard().inCheckmate() && getBoard().getLegalMoves().size() == 0);
    }
    void Game::claimDraw() {
        m_drawclaimed = true;
    }
    // Game :: GameOver
    bool Game::isGameOver() const {
        return getBoard().inCheckmate() || isDraw();
    }
    // PGN
    Game fromPGN(const string& filepath) {
        Game game;
        string source = (std::stringstream() << std::ifstream(filepath).rdbuf()).str();
        bool intag = false;
        bool inlinecomment = false;
        bool inscopedcomment = false;
        string movesource;
        for (char ch : source) {
            if (inlinecomment) {
                if (ch == '\n') {
                    inlinecomment = false;
                }
                continue;
            }
            else if (inscopedcomment) {
                if (ch == '}') {
                    inscopedcomment = false;
                }
                continue;
            }
            else if (intag) {
                if (ch == '\n') {
                    intag = false;
                }
                continue;
            }
            else if (ch == '[') {
                intag = true;
            }
            else if (ch == ';') {
                inlinecomment = true;
            }
            else if (ch == '{') {
                inscopedcomment = true;
            }
            else if (ch == '.') {
                while (0 < movesource.length() && std::isdigit(movesource.back())) {
                    movesource.pop_back();
                }
            }
            else if (ch == '\n') {
                movesource += ' ';
            }
            else {
                movesource += ch;
            }
        }
        List<string> movelist = split(movesource, " ");
        movelist.pop_back();
        for (const string& move : movelist) {
            std::cout << move << std::endl;
            game.doMove(move);
        }
        return game;
    }
}