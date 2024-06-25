#include "board.hpp"

namespace Chessmate {
    // get / set
    // :: get
    const Piece& Board::get(Square square) const {
        return squares.at(square);
    }
    const Piece& Board::get(Square square, Direction filedir, Direction rankdir) const {
        return get(addSquare(square, filedir, rankdir));
    }
    // :: set
    void Board::set(Square square, Piece piece) {
        squares.at(square) = piece;
    }

    // player / type
    Player Board::player(Square square) const {
        return get(square).player;
    }
    PieceType Board::type(Square square) const {
        return get(square).type;
    }

    // isEmpty
    bool Board::isEmpty(Square square) const {
        return get(square).isEmpty();
    }
    bool Board::isEmpty(Square square, Direction filedir, Direction rankdir) const {
        return get(square, filedir, rankdir).isEmpty();
    }
    // inBoundsAndEmpty
    bool Board::inBoundsAndEmpty(Square square, Direction filedir, Direction rankdir) const {
        return inBounds(square, filedir, rankdir) && isEmpty(square, filedir, rankdir);
    }

    // isFriendly
    bool Board::isFriendly(Square square) const {
        return get(square).player == active;
    }
    bool Board::isFriendly(Square square, Direction filedir, Direction rankdir) const {
        return get(square, filedir, rankdir).player == active;
    }
    // inBoundsAndFriendly
    bool Board::inBoundsAndFriendly(Square square, Direction filedir, Direction rankdir) const {
        return inBounds(square, filedir, rankdir) && isFriendly(square, filedir, rankdir);
    }

    // IsEnemy
    bool Board::isEnemy(Square square) const {
        return get(square).player == getEnemy(active);
    }
    bool Board::isEnemy(Square square, Direction filedir, Direction rankdir) const {
        return get(square, filedir, rankdir).player == getEnemy(active);
    }
    // inBoundsAndEnemy
    bool Board::inBoundsAndEnemy(Square square, Direction filedir, Direction rankdir) const {
        return inBounds(square, filedir, rankdir) && isEnemy(square, filedir, rankdir);
    }

    // inBoundsAndEquals
    bool Board::inBoundsAndEquals(Square square, Direction filedir, Direction rankdir, Piece piece) const {
        return inBounds(square, filedir, rankdir) && get(square, filedir, rankdir) == piece;
    }
}