#include "move.hpp"

namespace Chessmate {
    // Move
    // Move :: Constructor
    Move::Move()
        : Move(InvalidSquare, InvalidSquare, MoveFlag::None) {}
    Move::Move(MoveFlag flag)
        : Move(InvalidSquare, InvalidSquare, flag) {}
    Move::Move(Square origin, Square target)
        : Move(origin, target, MoveFlag::Normal) {}
    Move::Move(Square origin, Direction filedir, Direction rankdir)
        : Move(origin, addSquare(origin, filedir, rankdir), MoveFlag::Normal) {}
    Move::Move(Square origin, Square target, MoveFlag flag)
        : origin(origin), target(target), flag(flag) {}
    // Move :: Compare
    bool Move::operator==(const Move& other) const {
        return origin == other.origin && target == other.target && flag == other.flag;
    }
    bool Move::operator!=(const Move& other) const {
        return origin != other.origin || target != other.target || flag != other.flag;
    }
    // Move :: String
    string Move::toString(Piece piece) const {
        string str;
        if (!piece.isEmpty()) {
            str += fromPiece(piece);
            str += ' ';
        }
        str += fromSquare(origin) + "->" + fromSquare(target);
        switch (flag) {
        case MoveFlag::Normal:
            return str;
        case MoveFlag::DoubleAdvance:
            return str + " (+2)";
        case MoveFlag::EnPassant:
            return str + " (EP)";
        case MoveFlag::CastleStateK:
            return str + " (CK)";
        case MoveFlag::CastleStateQ:
            return str + " (CQ)";
        case MoveFlag::PromoteN:
            return str + " (PN)";
        case MoveFlag::PromoteB:
            return str + " (PB)";
        case MoveFlag::PromoteR:
            return str + " (PR)";
        case MoveFlag::PromoteQ:
            return str + " (PQ)";
        default:
            return "None";
        }
    }
}