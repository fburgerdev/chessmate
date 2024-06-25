#pragma once
#include "square.hpp"
#include "player.hpp"
#include "piece.hpp"

namespace Chessmate {
    struct Parse {
        // fromSquare / toSquare    
        static string fromSquare(Square square);
        static Square toSquare(const string& str);

        // fromPlayer / toPlayer
        static string fromPlayer(Player player);
        static Player toPlayer(const string& str);

        // toPieceType
        static PieceType toPieceType(char ch);
        // toPiece / fromPiece
        static Piece toPiece(char ch);
        static char fromPiece(Piece piece);
    };
}