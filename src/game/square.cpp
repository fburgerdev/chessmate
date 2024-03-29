#include "square.hpp"

namespace Chessmate {
    // InBounds
    bool inBounds(Square square, Direction filedir, Direction rankdir) {
        return 0 <= (square % 8) + filedir && (square % 8) + filedir < 8 && 0 <= (square / 8) + rankdir && (square / 8) + rankdir < 8;
    }
    // File / Rank
    int32 getFile(Square square) {
        return square % 8;
    }
    int32 getRank(Square square) {
        return square / 8;
    }
    // AddSquare
    Square addSquare(Square square, Direction filedir, Direction rankdir) {
        return square + 8 * rankdir + filedir;
    }
    // FromSquare / ToSquare
    string fromSquare(Square square) {
        string str;
        str += string("abcdefgh")[square % 8];
        str += string("87654321")[square / 8];
        return str;
    }
    Square toSquare(const string& str) {
        if (str.length() == 2) {
            if ('a' <= str[0] <= 'h' && '1' <= str[1] <= '8') {
                return 8 * ('8' - str[1]) + (str[0] - 'a');
            }
        }
        return NoSquare;
    }

}