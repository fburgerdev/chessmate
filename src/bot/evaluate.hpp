#pragma once
#include "game/board.hpp"

namespace Chessmate {
    // evaluatePieceType
    float evaluatePieceType(PieceType type);
    // evaluatePiece
    float evaluatePiece(Piece piece, Player active);
    // evaluateSquare
    float evaluateSquare(Square square, const Board& board);
    // evaluateBoard
    float evaluateBoard(const Board& board);
}