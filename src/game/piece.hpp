#pragma once
#include "player.hpp"

namespace Chessmate {
    // PieceType
    enum class PieceType {
        None = 0, Pawn, Knight, Bishop, Rook, Queen, King
    };
    // Piece
    struct Piece {
        // construtor
        Piece();
        Piece(PieceType type, Player player);

        // isEmpty
        bool isEmpty() const;
        
        // operator<=>
        auto operator<=>(const Piece& other) const = default;

        // type / player
        PieceType type;
        Player player;
    };
}