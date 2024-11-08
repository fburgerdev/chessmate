#include "move.hpp"
#include "parse.hpp"

namespace Chessmate {
    // constructor
    Move::Move()
        : Move(Piece(), NoSquare, NoSquare, MoveFlag::None) {}
    Move::Move(MoveFlag flag)
        : Move(Piece(), NoSquare, NoSquare, flag) {}
    Move::Move(Piece piece, Square origin, Square target)
        : Move(piece, origin, target, MoveFlag::Normal) {}
    Move::Move(Piece piece, Square origin, Direction filedir, Direction rankdir)
        : Move(piece, origin, addSquare(origin, filedir, rankdir), MoveFlag::Normal) {}
    Move::Move(Piece piece, Square origin, Square target, MoveFlag flag)
        : piece(piece), origin(origin), target(target), flag(flag) {}
    Move::Move(Piece piece, Square origin, Direction filedir, Direction rankdir, MoveFlag flag)
        : Move(piece, origin, addSquare(origin, filedir, rankdir), flag) {}
    
    // isValid
    bool Move::isValid() const {
        return !(piece.isEmpty() && origin == NoSquare && target == NoSquare && flag == MoveFlag::None);
    }

    // compare
    bool Move::operator==(const Move& other) const {
        return origin == other.origin && target == other.target && flag == other.flag;
    }
    bool Move::operator!=(const Move& other) const {
        return origin != other.origin || target != other.target || flag != other.flag;
    }
    
    // toAlgebraicNotation (Algebraic Notation - UCI Standard)
    string Move::toAlgebraicNotation() const {
        if (isValid()) {
            string out = Parse::fromSquare(origin) + Parse::fromSquare(target);
            switch (flag) {
            case MoveFlag::PromoteN:
                return out + "n";
            case MoveFlag::PromoteB:
                return out + "b";
            case MoveFlag::PromoteR:
                return out + "r";
            case MoveFlag::PromoteQ:
                return out + "q";
            default:
                return out;
            }
        }
        else {
            return "0000";
        }
    }
}