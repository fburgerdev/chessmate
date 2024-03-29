#pragma once
#include "game/board.hpp"

namespace Chessmate {
    // PieceType
    float evaluatePieceType(PieceType type);
    // Piece
    float evaluatePiece(Piece piece, Player active);
    // Square
    float evaluateSquare(Square square, const Board& board);
    // Board
    float evaluateBoard(const Board& board);
}