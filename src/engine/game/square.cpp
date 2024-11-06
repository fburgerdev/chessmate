#include "square.hpp"

namespace Chessmate {
    // inBounds
    bool inBounds(Square square, Direction filedir, Direction rankdir) {
        return 0 <= (square % 8) + filedir && (square % 8) + filedir < 8 && 0 <= (square / 8) + rankdir && (square / 8) + rankdir < 8;
    }

    // getFile / getRank
    int32 getFile(Square square) {
        return square % 8;
    }
    int32 getRank(Square square) {
        return square / 8;
    }

    // addSquare
    Square addSquare(Square square, Direction filedir, Direction rankdir) {
        return square + 8 * rankdir + filedir;
    }
}