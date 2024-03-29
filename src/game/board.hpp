#pragma once
#include "move.hpp"
#include "castle.hpp"

namespace Chessmate {
    // Board
    class Board {
    public:
        // Constructor
        Board(const string& fen);
        Board(const Board& oldboard, const Move& move);
        // FEN
        void fromFEN(const string& fen);
        string toFEN() const;
        // Algebraic Notation (UCI Standard)
        Move fromAlgebraicNotation(string notation) const;
        // Square
        // Square :: Get / Set
        const Piece& get(Square square) const;
        const Piece& get(Square square, Direction filedir, Direction rankdir) const;
        void set(Square square, Piece piece);
        // Square :: Player / Type
        Player player(Square square) const;
        PieceType type(Square square) const;
        // Square :: IsEmpty
        bool isEmpty(Square square) const;
        bool isEmpty(Square square, Direction filedir, Direction rankdir) const;
        bool inBoundsAndEmpty(Square square, Direction filedir, Direction rankdir) const;
        // Square :: IsFriendly
        bool isFriendly(Square square) const;
        bool isFriendly(Square square, Direction filedir, Direction rankdir) const;
        bool inBoundsAndFriendly(Square square, Direction filedir, Direction rankdir) const;
        // Square :: IsEnemy
        bool isEnemy(Square square) const;
        bool isEnemy(Square square, Direction filedir, Direction rankdir) const;
        bool inBoundsAndEnemy(Square square, Direction filedir, Direction rankdir) const;
        // Square :: InBoundsAndEquals
        bool inBoundsAndEquals(Square square, Direction filedir, Direction rankdir, Piece piece) const;
        // Move
        // Move :: Moveset
        List<Move> getMoveset() const;
        // Move :: LegalMoves
        List<Move> getLegalMoves() const;
        // Check
        // Check :: FindPiece
        Square findPiece(Piece piece) const;
        // Check :: AttackingSquares
        List<Square> getAttackingSquares(Square square, Player player) const;
        bool isSquareAttackedBy(Square square, Player player) const;
        // Check :: CanCaptureKing / IsKingAttacked
        bool canCaptureKing() const;
        bool isKingAttacked() const;
        // Check :: Check / Checkmate
        bool inCheck() const;
        bool inCheckmate() const;
        // Draw
        // Draw :: isDraw
        bool isDraw() const;
        // Draw :: HasSamePositions (According to FIDE)
        bool hasSamePositions(const Board& other) const;
        // String
        string toString() const;
        // Iterate
        auto begin();
        const auto begin() const;
        auto end();
        const auto end() const;
        // Member
        Player active;
        CastleState castle;
        Square enpassant;
        uint32 halfmove, fullmove;
        Array<Piece, 64> squares;
    };
}