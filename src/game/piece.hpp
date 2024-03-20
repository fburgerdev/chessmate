#pragma once
#include "player.hpp"

namespace Chessmate {
    // PieceType
    enum class PieceType {
        None = 0, Pawn, Knight, Bishop, Rook, Queen, King
    };
    // Piece
    struct Piece {
        // Constructor
        Piece();
        Piece(PieceType type, Player player);
        // IsEmpty
        bool isEmpty() const;
        // Compare
        bool operator==(const Piece& other) const;
        bool operator!=(const Piece& other) const;
        // Member
        PieceType type;
        Player player;
    };
    // ToPieceType
    PieceType toPieceType(char ch);
    // ToPiece / FromPiece
    Piece toPiece(char ch);
    char fromPiece(Piece piece);
}