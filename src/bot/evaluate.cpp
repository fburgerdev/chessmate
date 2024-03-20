#include "evaluate.hpp"

namespace Chessmate {
    // PieceType
    float evaluatePieceType(PieceType type) {
        switch (type) {
        case PieceType::Pawn:
            return 1;
        case PieceType::Knight:
            return 3;
        case PieceType::Bishop:
            return 3.25;
        case PieceType::Rook:
            return 5;
        case PieceType::Queen:
            return 9;
        default:
            return 0;
        }
    }
    // Piece
    float evaluatePiece(Piece piece, Player active) {
        return (piece.player == active ? +1 : -1) * evaluatePieceType(piece.type);
    }
    // Square
    // Square :: Pawn
    static constexpr Array<float, 64> pawnboard = {
        +00, +00, +00, +00, +00, +00, +00, +00,
        +50, +50, +50, +50, +50, +50, +50, +50,
        +10, +10, +20, +30, +30, +20, +10, +10,
        +05, +05, +10, +25, +25, +10, +05, +05
        +00, +00, +00, +20, +20, +00, +00, +00,
        +05, -05, -10, +00, +00, -10, -05, +05,
        +05, +10, +10, -20, -20, +10, +10, +05,
        +00, +00, +00, +00, +00, +00, +00, +00
    };
    // Square :: Knight
    static constexpr Array<float, 64> knightboard = {
        -50, -40, -30, -30, -30, -30, -40, -50,
        -40, -20, +00, +00, +00, +00, -20, -40,
        -30, +00, +10, +15, +15, +10, +00, -30,
        -30, +05, +15, +20, +20, +15, +05, -30,
        -30, +00, +15, +20, +20, +15, +00, -30,
        -30, +05, +10, +15, +15, +10, +05, -30,
        -40, -20, +00, +05, +05, +00, -20, -40,
        -50, -40, -30, -30, -30, -30, -40, -50
    };
    // Square :: Bishop
    static constexpr Array<float, 64> bishopboard = {
        -20, -10, -10, -10, -10, -10, -10, -20,
        -10, +00, +00, +00, +00, +00, +00, -10,
        -10, +00, +05, +10, +10, +05, +00, -10,
        -10, +05, +05, +10, +10, +05, +05, -10,
        -10, +00, +10, +10, +10, +10, +00, -10,
        -10, +10, +10, +10, +10, +10, +10, -10,
        -10, +05, +00, +00, +00, +00, +05, -10,
        -20, -10, -10, -10, -10, -10, -10, -20
    };
    // Square :: Rook
    static constexpr Array<float, 64> rookboard = {
        +00, +00, +00, +00, +00, +00, +00, +00,
        +05, +10, +10, +10, +10, +10, +10, +05,
        -05, +00, +00, +00, +00, +00, +00, -05,
        -05, +00, +00, +00, +00, +00, +00, -05,
        -05, +00, +00, +00, +00, +00, +00, -05,
        -05, +00, +00, +00, +00, +00, +00, -05,
        -05, +00, +00, +00, +00, +00, +00, -05,
        +00, +00, +00, +05, +05, +00, +00, +00
    };
    // Square :: Queen
    static constexpr Array<float, 64> queenboard = {
        -20, -10, -10, -05, -05, -10, -10, -20,
        -10, +00, +00, +00, +00, +00, +00, -10,
        -10, +00, +05, +05, +05, +05, +00, -10,
        -05, +00, +05, +05, +05, +05, +00, -05
        +00, +00, +05, +05, +05, +05, +00, -05,
        -10, +05, +05, +05, +05, +05, +00, -10,
        -10, +00, +05, +00, +00, +00, +00, -10,
        -20, -10, -10, -05, -05, -10, -10, -20
    };
    // Square :: King
    // Square :: King :: Midgame
    static constexpr Array<float, 64> midgamekingboard = {
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -20, -30, -30, -40, -40, -30, -30, -20,
        -10, -20, -20, -20, -20, -20, -20, -10,
        +20, +20, +00, +00, +00, +00, +20, +20,
        +20, +30, +10, +00, +00, +10, +30, +20
    };
    // Square :: King :: Endgame
    static constexpr Array<float, 64> endgamekingboard = {
        -50, -40, -30, -20, -20, -30, -40, -50,
        -30, -20, -10, +00, +00, -10, -20, -30,
        -30, -10, +20, +30, +30, +20, -10, -30,
        -30, -10, +30, +40, +40, +30, -10, -30,
        -30, -10, +30, +40, +40, +30, -10, -30,
        -30, -10, +20, +30, +30, +20, -10, -30,
        -30, -30, +00, +00, +00, +00, -30, -30,
        -50, -30, -30, -30, -30, -30, -30, -50
    };
    float evaluateSquare(Square square, const Board& board) {
        const float sign = (board.isFriendly(square) ? +1 : -1);
        const int32 index = (board.active == Player::White ? square : 63 - square);
        switch (board.type(square)) {
        case PieceType::Pawn:
            return sign * pawnboard.at(index);
        case PieceType::Knight:
            return sign * knightboard.at(index);
        case PieceType::Bishop:
            return sign * bishopboard.at(index);
        case PieceType::Rook:
            return sign * rookboard.at(index);
        case PieceType::Queen:
            return sign * queenboard.at(index);
        case PieceType::King:
            return sign * (board.fullmove < 20 ? midgamekingboard : endgamekingboard).at(index);
        default:
            return 0;
        }
    }
    // Board
    float evaluateBoard(const Board& board) {
        float eval = 0;
        for (Square square = 0; square < 64; ++square) {
            eval += evaluatePiece(board.get(square), board.active);
            eval += evaluateSquare(square, board);
        }
        return eval;
    }
}