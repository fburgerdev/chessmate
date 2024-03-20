#pragma once
#include "piece.hpp"
#include "square.hpp"

namespace Chessmate {
    // MoveFlag
    enum class MoveFlag {
        None = 0, Normal, DoubleAdvance, EnPassant, CastleStateK, CastleStateQ, PromoteN, PromoteB, PromoteR, PromoteQ, ClaimDraw
    };
    // Move
    struct Move {
        // Constructor
        Move();
        Move(MoveFlag flag);
        Move(Square origin, Square target);
        Move(Square origin, Direction filedir, Direction rankdir);
        Move(Square origin, Square target, MoveFlag flag);
        // Compare
        bool operator==(const Move& other) const;
        bool operator!=(const Move& other) const;
        // ToString
        string toString(Piece piece = Piece()) const;
        // Member
        Square origin;
        Square target;
        MoveFlag flag;
    };
}