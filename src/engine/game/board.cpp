#include "board.hpp"
#include "parse.hpp"

namespace Chessmate {
    // constructor
    Board::Board(const string& fen) {
        fromFEN(fen);
    }
    
    // fromAlgebraicNotation (Algebraic Notation - UCI Standard)
    Move Board::fromAlgebraicNotation(string notation) const {
        if (notation.length() == 4 || (notation.length() == 5 && string("qrbn").find(notation.back()) < string::npos)) {
            Square origin = Parse::toSquare(notation.substr(0, 2));
            Square target = Parse::toSquare(notation.substr(2, 2));
            std::function<bool(MoveFlag)> matchflag = [](MoveFlag flag){ return true; };
            if (notation.ends_with('q')) {
                matchflag = [](MoveFlag flag){ return flag == MoveFlag::PromoteQ; };
            }
            else if (notation.ends_with('r')) {
                matchflag = [](MoveFlag flag){ return flag == MoveFlag::PromoteR; };
            }
            else if (notation.ends_with('b')) {
                matchflag = [](MoveFlag flag){ return flag == MoveFlag::PromoteB; };
            }
            else if (notation.ends_with('n')) {
                matchflag = [](MoveFlag flag){ return flag == MoveFlag::PromoteN; };
            }
            for (const Move& move : getLegalMoves()) {
                if (origin == move.origin && target == move.target && matchflag(move.flag)) {
                    return move;
                }
            }
        }
        return Move();
    }

    // static
    Direction Board::PawnDirection(Player player) {
        return (player == Player::White ? -1 : 1);
    }
    Direction Board::PawnStartingPosition(Player player) {
        return (player == Player::White ? 6 : 1);
    }

    // toString
    string Board::toString() const {
        // squares
        List<string> lines;
        for (int32 rank = 0; rank < 8; ++rank) {
            string line = "| ";
            for (int32 file = 0; file < 8; ++file) {
                line += Parse::fromPiece(get((8 * rank + file)));
                line += ' ';
            }
            if (rank == 0) {
                line += " Gamestate";
            }
            else if (rank == 2) {
                line += " Active: ";
                line += (active == Player::White ? "White" : (active == Player::Black ? "Black" : "None"));
            }
            else if (rank == 3) {
                line += " Castle: ";
                if (castle[Castle::WhiteKingside]) {
                    line += 'K';
                }
                if (castle[Castle::WhiteQueenside]) {
                    line += 'Q';
                }
                if (castle[Castle::BlackKingside]) {
                    line += 'k';
                }
                if (castle[Castle::BlackQueenside]) {
                    line += 'q';
                }
            }
            else if (rank == 4) {
                line += " Halfmove: ";
                line += std::to_string(halfMoves); 
            }
            else if (rank == 5) {
                line += " Fullmove: ";
                line += std::to_string(fullMoves); 
            }
            lines.push_back(line);
        }

        // max length
        address maxlength = 0;
        for (const string& line : lines) {
            maxlength = std::max(line.length(), maxlength);
        }
        
        // out
        string out;
        for (int32 i = 0; i < maxlength + 2; ++i) {
            out += '-';
        }
        out += '\n';
        for (const string& line : lines) {
            out += line;
            for (int32 i = line.length(); i < maxlength; ++i) {
                out += ' ';
            }
            out += " |\n";
        }
        for (int32 i = 0; i < maxlength + 2; ++i) {
            out += '-';
        }
        out += '\n';
        return out;
    }

    // iterate
    auto Board::begin() {
        return squares.begin();
    }
    const auto Board::begin() const {
        return squares.begin();
    }
    auto Board::end() {
        return squares.end();
    }
    const auto Board::end() const {
        return squares.end();
    }
}