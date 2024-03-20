#pragma once
#include "common.hpp"

namespace Chessmate {
    // Square / Direction
    using Square = int32;
    using Direction = int32;
    // InvalidSquare
    static inline constexpr Square InvalidSquare = -1;
    // InBounds
    bool inBounds(Square square, Direction filedir, Direction rankdir);
    // File / Rank
    int32 getFile(Square square);
    int32 getRank(Square square);
    // AddSquare
    Square addSquare(Square square, Direction filedir, Direction rankdir);
    // FromSquare / ToSquare
    string fromSquare(Square square);
    Square toSquare(const char charsquare[2]);
    Square toSquare(const string& charsquare);
}