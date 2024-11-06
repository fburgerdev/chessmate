#include "evaluate.hpp"

namespace Chessmate {
    // evaluatePieceType
    float evaluatePieceType(PieceType type) {
        switch (type) {
        case PieceType::Pawn:
            return 100;
        case PieceType::Knight:
            return 300;
        case PieceType::Bishop:
            return 325;
        case PieceType::Rook:
            return 500;
        case PieceType::Queen:
            return 900;
        default:
            return 0;
        }
    }

    // evaluatePiece
    float evaluatePiece(Piece piece, Player active) {
        return (piece.player == active ? +1 : -1) * evaluatePieceType(piece.type);
    }
    
    // square
    // :: pawn
    static constexpr Array<float, 64> PawnBoard = {
        +00, +00, +00, +00, +00, +00, +00, +00,
        +50, +50, +50, +50, +50, +50, +50, +50,
        +10, +10, +20, +30, +30, +20, +10, +10,
        +05, +05, +10, +25, +25, +10, +05, +05
        +00, +00, +00, +20, +20, +00, +00, +00,
        +05, -05, -10, +00, +00, -10, -05, +05,
        +05, +10, +10, -20, -20, +10, +10, +05,
        +00, +00, +00, +00, +00, +00, +00, +00
    };
    // :: knight
    static constexpr Array<float, 64> KnightBoard = {
        -50, -40, -30, -30, -30, -30, -40, -50,
        -40, -20, +00, +00, +00, +00, -20, -40,
        -30, +00, +10, +15, +15, +10, +00, -30,
        -30, +05, +15, +20, +20, +15, +05, -30,
        -30, +00, +15, +20, +20, +15, +00, -30,
        -30, +05, +10, +15, +15, +10, +05, -30,
        -40, -20, +00, +05, +05, +00, -20, -40,
        -50, -40, -30, -30, -30, -30, -40, -50
    };
    // :: bishop
    static constexpr Array<float, 64> BishopBoard = {
        -20, -10, -10, -10, -10, -10, -10, -20,
        -10, +00, +00, +00, +00, +00, +00, -10,
        -10, +00, +05, +10, +10, +05, +00, -10,
        -10, +05, +05, +10, +10, +05, +05, -10,
        -10, +00, +10, +10, +10, +10, +00, -10,
        -10, +10, +10, +10, +10, +10, +10, -10,
        -10, +05, +00, +00, +00, +00, +05, -10,
        -20, -10, -10, -10, -10, -10, -10, -20
    };
    // :: rook
    static constexpr Array<float, 64> RookBoard = {
        +00, +00, +00, +00, +00, +00, +00, +00,
        +05, +10, +10, +10, +10, +10, +10, +05,
        -05, +00, +00, +00, +00, +00, +00, -05,
        -05, +00, +00, +00, +00, +00, +00, -05,
        -05, +00, +00, +00, +00, +00, +00, -05,
        -05, +00, +00, +00, +00, +00, +00, -05,
        -05, +00, +00, +00, +00, +00, +00, -05,
        +00, +00, +00, +05, +05, +00, +00, +00
    };
    // :: queen
    static constexpr Array<float, 64> QueenBoard = {
        -20, -10, -10, -05, -05, -10, -10, -20,
        -10, +00, +00, +00, +00, +00, +00, -10,
        -10, +00, +05, +05, +05, +05, +00, -10,
        -05, +00, +05, +05, +05, +05, +00, -05
        +00, +00, +05, +05, +05, +05, +00, -05,
        -10, +05, +05, +05, +05, +05, +00, -10,
        -10, +00, +05, +00, +00, +00, +00, -10,
        -20, -10, -10, -05, -05, -10, -10, -20
    };
    // :: king
    static constexpr Array<float, 64> MidgameKingBoard = {
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -30, -40, -40, -50, -50, -40, -40, -30,
        -20, -30, -30, -40, -40, -30, -30, -20,
        -10, -20, -20, -20, -20, -20, -20, -10,
        +20, +20, +00, +00, +00, +00, +20, +20,
        +20, +30, +10, +00, +00, +10, +30, +20
    };
    static constexpr Array<float, 64> EndgameKingBoard = {
        -50, -40, -30, -20, -20, -30, -40, -50,
        -30, -20, -10, +00, +00, -10, -20, -30,
        -30, -10, +20, +30, +30, +20, -10, -30,
        -30, -10, +30, +40, +40, +30, -10, -30,
        -30, -10, +30, +40, +40, +30, -10, -30,
        -30, -10, +20, +30, +30, +20, -10, -30,
        -30, -30, +00, +00, +00, +00, -30, -30,
        -50, -30, -30, -30, -30, -30, -30, -50
    };
    // :: evaluateSquare
    float evaluateSquare(Square square, const Board& board) {
        const float sign = (board.isFriendly(square) ? +1 : -1);
        const int32 index = (board.active == Player::White ? square : 63 - square);
        switch (board.type(square)) {
        case PieceType::Pawn:
            return sign * PawnBoard.at(index);
        case PieceType::Knight:
            return sign * KnightBoard.at(index);
        case PieceType::Bishop:
            return sign * BishopBoard.at(index);
        case PieceType::Rook:
            return sign * RookBoard.at(index);
        case PieceType::Queen:
            return sign * QueenBoard.at(index);
        case PieceType::King:
            return sign * (board.fullMoves < 20 ? MidgameKingBoard : EndgameKingBoard).at(index);
        default:
            return 0;
        }
    }

    // evaluateBoard
    float evaluateBoard(const Board& board) {
        float eval = 0;
        for (Square square = 0; square < 64; ++square) {
            eval += evaluatePiece(board.get(square), board.active);
            eval += evaluateSquare(square, board);
        }
        return eval;
    }
}