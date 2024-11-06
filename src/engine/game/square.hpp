#pragma once
#include "common.hpp"

namespace Chessmate {
    // Square / Direction
    using Square = int32;
    using Direction = int32;
    
    // NoSquare
    static inline constexpr Square NoSquare = -1;

    // inBounds
    bool inBounds(Square square, Direction filedir, Direction rankdir);
    
    // getFile / getRank
    int32 getFile(Square square);
    int32 getRank(Square square);

    // addSquare
    Square addSquare(Square square, Direction filedir, Direction rankdir);
}