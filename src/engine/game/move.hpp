#pragma once
#include "piece.hpp"
#include "square.hpp"

namespace Chessmate {
    // MoveFlag
    enum class MoveFlag {
        None = 0, Normal, DoubleAdvance, EnPassant, CastleK, CastleQ, PromoteN, PromoteB, PromoteR, PromoteQ
    };
    // Move
    struct Move {
        // constructor
        Move();
        Move(MoveFlag flag);
        Move(Piece piece, Square origin, Square target);
        Move(Piece piece, Square origin, Direction filedir, Direction rankdir);
        Move(Piece piece, Square origin, Square target, MoveFlag flag);
        Move(Piece piece, Square origin, Direction filedir, Direction rankdir, MoveFlag flag);

        // isValid
        bool isValid() const;
        
        // compare
        bool operator==(const Move& other) const;
        bool operator!=(const Move& other) const;
        
        // toAlgebraicNotation (Algebraic Notation - UCI Standard)
        string toAlgebraicNotation() const;
        
        // member
        Piece piece;
        Square origin, target;
        MoveFlag flag;
    };
}