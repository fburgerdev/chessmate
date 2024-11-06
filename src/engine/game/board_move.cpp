#include "board.hpp"
#include "parse.hpp"

namespace Chessmate {
    // constructor
    Board::Board(const Board& oldboard, const Move& move)
        : Board(oldboard) {
        // castle privileges
        // :: white king moves
        if (move.origin == Parse::toSquare("e1")) {
            castle[Castle::WhiteKingside] = false;
            castle[Castle::WhiteQueenside] = false;
        }
        // :: black king moves
        else if (move.origin == Parse::toSquare("e8")) {
            castle[Castle::BlackKingside] = false;
            castle[Castle::BlackQueenside] = false;
        }
        // :: white kingside rook moves
        else if (move.origin == Parse::toSquare("h1")) {
            castle[Castle::WhiteKingside] = false;
        }
        // :: white queenside rook moves
        else if (move.origin == Parse::toSquare("a1")) {
            castle[Castle::WhiteQueenside] = false;
        }
        // :: black kingside rook moves
        else if (move.origin == Parse::toSquare("h8")) {
            castle[Castle::BlackKingside] = false;
        }
        // :: black queenside rook moves
        else if (move.origin == Parse::toSquare("a8")) {
            castle[Castle::BlackQueenside] = false;
        }

        // perform castle
        // :: kingside
        if (move.flag == MoveFlag::CastleK) {
            // move rook
            set((active == Player::White) ? Parse::toSquare("f1") : Parse::toSquare("f8"), Piece(PieceType::Rook, active));
            set((active == Player::White) ? Parse::toSquare("h1") : Parse::toSquare("h8"), Piece());
        }
        // :: queenside
        else if (move.flag == MoveFlag::CastleQ) {
            // move rook
            set((active == Player::White) ? Parse::toSquare("d1") : Parse::toSquare("d8"), Piece(PieceType::Rook, active));
            set((active == Player::White) ? Parse::toSquare("a1") : Parse::toSquare("a8"), Piece());
        }
 
        // enPassant
        if (move.flag == MoveFlag::EnPassant) {
            set(enPassant, Piece());
        }

        // halfMoves (piece is captured or pawn advances)
        if (isEnemy(move.target) || type(move.origin) == PieceType::Pawn) {
            halfMoves = 0;
        }
        else {
            halfMoves += 1;
        }

        // move piece
        // :: promotion
        if (move.flag == MoveFlag::PromoteN) {
            set(move.target, Piece(PieceType::Knight, active));
        }
        else if (move.flag == MoveFlag::PromoteB) {
            set(move.target, Piece(PieceType::Bishop, active));
        }
        else if (move.flag == MoveFlag::PromoteR) {
            set(move.target, Piece(PieceType::Rook, active));
        }
        else if (move.flag == MoveFlag::PromoteQ) {
            set(move.target, Piece(PieceType::Queen, active));
        }
        // :: normal
        else {
            set(move.target, get(move.origin));
        }
        set(move.origin, Piece());

        // enPassant
        if (move.flag == MoveFlag::DoubleAdvance) {
            enPassant = addSquare(move.origin, 0, PawnDirection(active));
        }
        else {
            enPassant = NoSquare;
        }
        
        // fullMoves (both players made a turn)
        if (active == Player::Black) {
            fullMoves += 1;
        }

        // active
        active = getEnemy(active);
    }
}