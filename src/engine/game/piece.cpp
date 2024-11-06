#include "piece.hpp"

namespace Chessmate {
    // constructor
    Piece::Piece()
        : Piece(PieceType::None, Player::None) {}
    Piece::Piece(PieceType type, Player player)
        : type(type), player(player) {}
    
    // isEmpty
    bool Piece::isEmpty() const {
        return type == PieceType::None && player == Player::None;
    }
}