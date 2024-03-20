#include "formatresults.hpp"
#include "bot/minimax.hpp"
#include <sstream> // stringstream

namespace Chessmate {
    string formatResults(const Game& game, int32 depth) {
        const Board& board = game.getBoard();
        std::stringstream stream;
        stream << '{';
        // Board
        stream << "board:{";
        // Board :: FEN
        stream << "fen:\"" << board.toFEN() << "\"";
        stream << ',';
        // Board :: Active
        stream << "active:\"" << fromPlayer(board.active)  << "\"";
        stream << ',';
        // Board :: CastleState
        string castle;
        if (board.castle[Castle::WhiteKingside]) {
            castle += "\"K\",";
        }
        if (board.castle[Castle::WhiteQueenside]) {
            castle += "\"Q\",";
        }
        if (board.castle[Castle::BlackKingside]) {
            castle += "\"k\",";
        }
        if (board.castle[Castle::BlackQueenside]) {
            castle += "\"q\",";
        }
        if (castle.length()) {
            castle.pop_back();
        }
        stream << "castle:[" << castle << "]";
        stream << ',';
        // Halfmove
        stream << "halfmove:" << board.halfmove;
        stream << ",";
        // Fullmove
        stream << "fullmove:" << board.fullmove;
        stream << '}';
        stream << ',';
        // Info
        stream << "info:{";
        // Info :: GameState
        stream << "gamestate:";
        if (board.getLegalMoves().size()) {
            stream << "\"ingame\"";
        }
        else {
            if (board.inCheck()) {
                stream << "\"checkmate\""; 
            }
            else {
                stream << "\"draw\"";
            }
        }
        stream << ',';
        // Info :: InCheck
        stream << "incheck:" << (board.isKingAttacked() ? "true" : "false");
        stream << ',';
        // Info :: LegalMoves
        string legalmoves;
        for (const Move& move : board.getLegalMoves()) {
            legalmoves += '\"';
            legalmoves += fromSquare(move.origin);
            legalmoves += "->";
            legalmoves += fromSquare(move.target);
            legalmoves += '\"';
            legalmoves += ',';
        }
        if (legalmoves.length()) {
            legalmoves.pop_back();
        }
        stream << "legalmoves:[" << legalmoves << "]";
        stream << '}';
        stream << ',';
        // Bot
        if (depth) {
            stream << "bestmove:\"" << minimax(board, depth).move.toString() << "\"";
        }
        stream << '}';
        return stream.str();
    }
}